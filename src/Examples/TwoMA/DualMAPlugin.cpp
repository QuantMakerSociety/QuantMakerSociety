#include "stdafx.h"
//
#include "Globals.h"

void DualMAPlugin_onTick(QMSServer &svr, const qms_barData &bd);

void DualMAPlugin_onInit(QMSServer &svr, const qms_barData &bd);

void DualMAPlugin_onShutdown(QMSServer &svr, const qms_barData &bd);

void DualMAPlugin_onOrderFilled(QMSServer &svr, const qms_barData &bd,
                                const void *pExtra);

void DualMAPlugin_onOrderError(QMSServer &svr, const qms_barData &bd,
                               const void *pExtra);

void DualMAPlugin_onLimitPlaced(QMSServer &svr, const qms_barData &bd,
                                const void *pExtra);

void DualMAPlugin_onLimitCancelled(QMSServer &svr, const qms_barData &bd,
                                   const void *pExtra);

void DualMAPlugin_onLimitError(QMSServer &svr, const qms_barData &bd,
                               const void *pExtra);

void DualMAPlugin_onLimitEdited(QMSServer &svr, const qms_barData &bd,
                                const void *pExtra);

void DualMAPlugin(QMSServer &svr, int msg, const qms_barData &bd,
                  const void *pExtra) {
  switch (msg) {
  case eQMSQuantMsg_tick:
    return DualMAPlugin_onTick(svr, bd);
  case eQMSQuantMsg_init:
    return DualMAPlugin_onInit(svr, bd);
  case eQMSQuantMsg_shutdown:
    return DualMAPlugin_onShutdown(svr, bd);

  case eQMSQuantMsg_onOrderFilled:
    return DualMAPlugin_onOrderFilled(svr, bd, pExtra);
  case eQMSQuantMsg_onOrderError:
    return DualMAPlugin_onOrderError(svr, bd, pExtra);
  case eQMSQuantMsg_onLimitPlaced:
    return DualMAPlugin_onLimitPlaced(svr, bd, pExtra);
  case eQMSQuantMsg_onLimitError:
    return DualMAPlugin_onLimitError(svr, bd, pExtra);
  case eQMSQuantMsg_onLimitCancelled:
    return DualMAPlugin_onLimitCancelled(svr, bd, pExtra);
  case eQMSQuantMsg_onLimitEdited:
    return DualMAPlugin_onLimitEdited(svr, bd, pExtra);
  default:
    return;
  }
}

void DualMAPlugin_onTick(QMSServer &svr, const qms_barData &bd) {
  if (!bd.pQuant) {
    return;
  }
  qms_quant &who = *bd.pQuant;

  if (!bd.ptr || !bd.acct || !bd.symbol || !bd.pExposure) {
    svr.Q_UpdateStatusMsg(who, "\2"
                               "\xc"
                               "INVALID PARAMETERS");
    return;
  }

  qms_assert(0 < bd.price.tim);

  std::stringstream ss;

  QuantState &qs = *(QuantState *)bd.ptr;
  const qms_account &a = *bd.acct;
  const qms_symbol &s = *bd.symbol;
  const qms_exposure &e = *bd.pExposure;

  double bid = bd.price.bid;
  double ask = bd.price.ask;

  ss << s.name << ": bid=" << bd.price.bid << ", ask=" << bd.price.ask << "\n";
  ss << "Account " << a.name << "(" << a.id << ") : Balance = " << a.balance
     << ", Equity = " << a.equity << "\n";
  ss << "Exposure vol = " << e.volume << ", Q= " << e.exposure
     << ", USD = " << e.exposureUSD << ", M=" << e.margin
     << ", MUSD = " << e.marginUSD << "\n";

  qs.pAccount = bd.acct;
  qs.pSymbol = bd.symbol;

  int lastSignal = qs.lastSignal;

  const qms_itemArray &cfg = bd.cfg;

  int ids = bd.symbol->id;
  int ida = bd.acct->id;

  if (cfg.count != eCfgItem_count) {
    // Invalid number of items in configuration.
    ss << "\2"
          "\xc"
          "INVALID CONFIGURATION\n";
    svr.Q_UpdateStatusMsg(who, ss.str().c_str());
    return;
  }

  if (ida != qs.ida) {
    // Account chancged.
    qs.ida = ida;
  }

  if (ids != qs.ids) {
    // Symbol changed.
    qs.ids = ids;
  }

  double volume = cfg[eCfgItem_volume].val.dbl;
  int fastMA = cfg[eCfgItem_fastMA].val.i32;
  int fastPeriod = cfg[eCfgItem_fastMAPeriod].val.i32;
  int slowMA = cfg[eCfgItem_slowMA].val.i32;
  int slowPeriod = cfg[eCfgItem_slowMAPeriod].val.i32;

  double td = cfg[eCfgItem_tradeDistance].val.dbl;
  double sl = cfg[eCfgItem_stopLoss].val.dbl;
  double tp = cfg[eCfgItem_takeProfit].val.dbl;

  if (1E-6 > td) {
    td = 0; // Use markets instead of limits for main trade.
  }

  if (1E-6 > sl) {
    sl = 0; // No stop loss.
  }

  if (1E-6 > tp) {
    tp = 0; // No take profit.
  }

  if (1E-6 < fabs(td - qs.td)) {
    // Distance to place a limit changed.
    qs.td = td;
    qs.useLimits = (1E-6 < td);
  }

  if (1E-6 < fabs(sl - qs.sl)) {
    // Stop loss changed.
    qs.sl = sl;
    qs.useStopLoss = (1E-6 < sl);
  }

  if (1E-6 < fabs(tp - qs.tp)) {
    // Take profit changed.
    qs.tp = tp;
    qs.useTakeProfit = (1E-6 < tp);
  }

  if (1E-6 < fabs(volume - qs.volume)) {
    // Volume changed.
    qs.volume = volume;
  }

  if (fastPeriod > slowPeriod) {
    // Swap fast and slow periods.
    int x = slowPeriod;
    slowPeriod = fastPeriod;
    fastPeriod = x;
  }

  if (fastMA != qs.fastMA) {
    // Change fast moving average.
    qs.fastMA = fastMA;
  }

  if (fastPeriod != qs.fastMAPeriod) {
    // Fast period changed.
    qs.fastMAPeriod = fastPeriod;
  }

  if (slowMA != qs.slowMA) {
    // Slow moving average mode changed.
    qs.slowMA = slowMA;
  }

  if (slowPeriod != qs.slowMAPeriod) {
    // Slow period changed.
    qs.slowMAPeriod = slowPeriod;
  }

  int count = bd.count; // Number of bars.

  if (count < (slowPeriod + 5)) {
    // Want at least 5 bars more than the slow period.
    return;
  }

  // We will only use the close price.
  const double *in = bd.close;
  const int64_t * pt = bd.time;

  // And will track only up to 1024 bars.
  if (count > 1024) {
    in += (count - 1024);
	pt += (count - 1024);
    count = 1024;
  }
  int64_t t0 = pt[count - 1];

  double *outFast = qs.outFast;
  double *outSlow = qs.outSlow;

  int fastBeg = 0;
  int fastCount = 0;

  int r = TA_MA(0, count - 1, in, fastPeriod, (TA_MAType)fastMA, &fastBeg,&fastCount, outFast);
  if (fastCount < 4) {
    return;
  }

  int slowBeg = 0;
  int slowCount = 0;
  r = TA_MA(0, count - 1, in, slowPeriod, (TA_MAType)slowMA, &slowBeg,
            &slowCount, outSlow);
  if (fastCount < 4) {
    return;
  }

  double d[2];
  double fast[2];
  double slow[2];

  for (int i = 0, j = fastCount - 1, k = slowCount - 1; i < 2; i++, j--, k--) {
    fast[i] = outFast[j];
    slow[i] = outSlow[k];
    d[i] = fast[i] - slow[i];
  }

  ss << "time=" << bd.price.tim << ", t0 =" << t0 << ", Count=" << bd.count << "\n";
  ss << "F0=" << fast[0] << ", F1=" << fast[1] << "\n";
  ss << "S0=" << slow[0] << ", S1=" << slow[1] << "\n";
  ss << 'D0=' << d[0] << ", D1=" << d[1] << "\n";

  char stat[17] = {0};
  {
    int n = 16;
    if (fastCount < n) {
      n = fastCount;
    }

    if (slowCount < n) {
      n = slowCount;
    }

    int d1;
    for (int i = 0, j = fastCount - n, k = slowCount - n; i < n; i++) {
      double delta = outFast[j] - outSlow[k];
      if (0 > delta) {
        if (0 < d1) {
          stat[i] = 'S';
        } else {
          stat[i] = 's';
        }
        d1 = -1;
      } else if (0 < delta) {
        stat[i] = 'l';
        if (0 > d1) {
          stat[i] = 'L';
        } else {
          stat[i] = 'l';
        }
        d1 = 1;
      } else {
        stat[i] = '.';
        d1 = 0;
      }
    }

    stat[n] = 0;
  }
  ss << stat << "\n";

  int signal = 0;

  if (0 > d[1]) {
    // Short
    signal = -1;
    if (0 < lastSignal) {
      signal = -2;
    }
  } else if (0 < d[1]) {
    // Long
    signal = 1;
    if (0 > lastSignal) {
      signal = 2;
    }
  }

  qs.lastSignal = signal;

  double expVol = e.volume;

  static const double digitsToPips[] = {
      1.0, 0.1, 0.01, 0.001, 0.0001, 0.0001, 0.0001,
  };
  double onePip = digitsToPips[bd.symbol->nDigits];

  if (0 > signal) {
    if (-1 > signal) {
      ss << "\2"
            "\xc"
            "SHORT SIGNAL\n";
    } else {
      ss << "\2"
            "\xc"
            "Short\n";
    }
  } else if (0 < signal) {
    if (1 < signal) {
      ss << "\2"
            "\xa"
            "LONG SIGNAL\n";
    } else {
      ss << "\2"
            "\xa"
            "Long\n";
    }
  }

  if (-2 < signal && 2 > signal) {
    // No signal.

    // Keep track of stop loss. move it if necessary.
    if (qs.slId) {
      double center = 0.5 * (bd.price.bid + bd.price.ask);
      double delta = qs.slLastUpdatePrice - center;

      if (0 > qs.slSide) {
        double delta = qs.slLastUpdatePrice - bd.price.bid;
        if (delta > qs.slPips) {
          // Need to move it.
          double newPrice = bd.price.bid + qs.slPips;
          int r = svr.Q_EditLimit(who, qs.slId, newPrice, 0);
          qs.slLastUpdatePrice = newPrice;
        }
      } else {
        double delta = bd.price.ask - qs.slLastUpdatePrice;
        if (delta > qs.slPips) {
          // Need to move it.
          double newPrice = bd.price.ask - qs.slPips;
          int r = svr.Q_EditLimit(who, qs.slId, newPrice, 0);
          qs.slLastUpdatePrice = newPrice;
        }
      }
    }

	ss << "\n";

    svr.Q_UpdateStatusMsg(who, ss.str().c_str());
    return;
  }

  if (-1 > signal) {
    // Short signal.
    u64_t oid;

    double vol = -volume;

    if (1E-6 < fabs(expVol)) {
      if (0 < expVol) {
        svr.Q_PlaceMarketOrder(who, ida, ids, oid.x64, -expVol, 0,
                               "CLOSE_EXPOSURE");
        ++oid;
        ss << "Closed Exposure: " << expVol << "\n";
      }
    }

    qs.waitingId = oid.x64;

    qms_quant &who = *bd.pQuant;

    if (qs.slId) {
      svr.Q_CancelLimit(who, qs.slId);
      ss << "Canceled Stop Loss: " << qs.slId << "\n";
      qs.slId = 0;
    }

    if (qs.tpId) {
      svr.Q_CancelLimit(who, qs.tpId);
      ss << "Canceled Take Profit: " << qs.tpId << "\n";
      qs.tpId = 0;
    }

    int r;

    double price = bd.price.bid;
    if (qs.useLimits) {
      double pips = qs.td * onePip;
      price += pips;
      r = svr.Q_PlaceLimit(who, ida, ids, oid.x64, 2, 0, vol, price, 0,
                           "SELL_LIMIT");
      ss << "Placed sell limit: " << oid.x64 << " : vol = " << vol
         << ", result = " << r << "\n";
    } else {
      price;
      r = svr.Q_PlaceMarketOrder(who, ida, ids, oid.x64, vol, 0, "SELL_MARKET");
      ss << "Placed Market order: " << oid.x64 << " : vol = " << vol
         << ", result = " << r << "\n";
    }

    if (0 > r) {
      ss << "ERROR IN ORDER\n";
      return;
    }

    if (qs.useTakeProfit) {
      ++oid;
      double pips = qs.tp * onePip;
      double x = price - pips;
      qs.tpId =
          svr.Q_PlaceLimit(who, ida, ids, oid.x64, 2, 9, -vol, x, 0, "SELL_TP");
      qs.tpOrderId = oid.x64;
      ss << "Placed Sell Take Profit Sell limit: " << oid.x64
         << " : vol = " << -vol << ", price = " << x << "\n";
    }

    if (qs.useStopLoss) {
      ++oid;
      double pips = qs.sl * onePip;
      double x = price + pips;
      qs.slId =
          svr.Q_PlaceLimit(who, ida, ids, oid.x64, 3, 9, -vol, x, x, "SELL_SL");
      qs.slOrderId = oid.x64;
      qs.slSide = -1;
      qs.slLastUpdatePrice = x;
      qs.slPips = pips;
      ss << "Placed Sell Stop loss: Sell Stop: " << oid.x64
         << " : vol = " << -vol << ", price = " << x << "\n";
    }

    svr.Q_UpdateStatusMsg(who, ss.str().c_str());
    Log("%s\n", ss.str().c_str());
    return;
  }

  if (1 < signal) {
    // Long signal.
    u64_t oid;

    double vol = volume;
    if (1E-6 < fabs(expVol)) {
      if (0 > expVol) {
        svr.Q_PlaceMarketOrder(who, ida, ids, oid.x64, -expVol, 0,
                               "CLOSE_EXPOSURE");
        ++oid;
        ss << "Closed Exposure: " << expVol << "\n";
      }
    }
    qs.waitingId = oid.x64;

    double price = bd.price.ask;

    int r;

    if (qs.slId) {
      svr.Q_CancelLimit(who, qs.slId);
      ss << "Canceled SL: " << qs.slId << "\n";
      qs.slId = 0;
    }

    if (qs.tpId) {
      svr.Q_CancelLimit(who, qs.tpId);
      ss << "Canceled TP: " << qs.slId << "\n";
      qs.tpId = 0;
    }

    if (qs.useLimits) {
      double pips = qs.td * onePip;
      price -= pips;
      r = svr.Q_PlaceLimit(who, ida, ids, oid.x64, 2, 9, vol, price, 0,
                           "BUY_LIMIT");
      ss << "Placed buy limit: " << oid.x64 << " : vol = " << vol
         << ", result = " << r << "\n";
    } else {
      r = svr.Q_PlaceMarketOrder(who, ida, ids, oid.x64, vol, 0, "BUY_MARKET");
      ss << "Placed Sell Market: " << oid.x64 << " : vol = " << vol
         << ", price = " << price << "\n";
    }

    if (0 > r) {
      ss << "ERROR IN ORDER\n";
      return;
    }

    if (qs.useTakeProfit) {
      double pips = qs.tp * onePip;
      double x = price + pips;
      ++oid;
      qs.tpId =
          svr.Q_PlaceLimit(who, ida, ids, oid.x64, 2, 9, -vol, x, 0, "BUY_TP");
      ss << "Placed Buy Take Profit Sell limit: " << oid.x64
         << " : vol = " << -vol << ", price = " << x << "\n";
      qs.tpOrderId = oid.x64;
    }

    if (qs.useStopLoss) {
      double pips = qs.sl * onePip;
      double x = price - pips;
      ++oid;
      qs.slId =
          svr.Q_PlaceLimit(who, ida, ids, oid.x64, 3, 9, -vol, x, x, "BUY_SL");
      qs.slOrderId = oid.x64;
      qs.slSide = 1;
      qs.slLastUpdatePrice = x;
      qs.slPips = pips;
      ss << "Placed Buy Take Profit Sell limit: " << oid.x64
         << " : vol = " << -vol << ", price = " << x << "\n";
    }

    svr.Q_UpdateStatusMsg(who, ss.str().c_str());
    Log("%s\n", ss.str().c_str());
  }
}

void DualMAPlugin_onInit(QMSServer &svr, const qms_barData &bd) {
  bd.ptr = NewQuantState();
  const qms_itemArray &cfg = bd.cfg;
  std::stringstream ss;

  qms_quant &who = *bd.pQuant;

  ss << "Started TWOMA: " << bd.symbol->name << ", acct=" << bd.acct->name
     << "(" << bd.acct->id << "): ";
  ss << ",spb= " << bd.secondsPerBar;
  ss << ",fast= " << cfg[0].val.i32;
  ss << ", slow=" << cfg[1].val.i32 << "\n";

  Log("%s\n", ss.str().c_str());
  svr.Q_UpdateStatusMsg(who, ss.str().c_str());
  return;
}

void DualMAPlugin_onShutdown(QMSServer &svr, const qms_barData &bd) {
  FreeQuantState((QuantState *)bd.ptr);
  bd.ptr = 0;
}

void DualMAPlugin_onOrderFilled(QMSServer &svr, const qms_barData &bd,
                                const void *pExtra) {
  if (!pExtra || !bd.ptr) {
    return;
  }
  QuantState &qs = *(QuantState *)bd.ptr;
  const qms_deal &d = *(const qms_deal *)pExtra;
  qms_quant &who = *bd.pQuant;
  Log("Filled order %d in account %d, sbol=%d, price=%f, strat=%I64u\n", d.id,
      d.idAcct, d.idSymbol, d.price, d.stratId);
  Log("qs.slOrderId = %I64u, slId = %u\n", qs.slOrderId, qs.slId);
  Log("qs.tpOrderId = %I64u, tpId = %u\n", qs.tpOrderId, qs.slId);
  Log("Desc=\"%s\"\n", d.description);

  if (d.stratId == qs.slOrderId) {
    // Hit stop loss. Cancel take profit.
    qs.slOrderId = 0;
    qs.slId = 0;

    if (qs.tpId) {
      svr.Q_CancelLimit(who, qs.tpId);
      qs.tpId = 0;
      qs.tpOrderId = 0;
    }
  } else if (d.stratId == qs.tpId) {
    // Hit take profit. Cancel stop loss.
    qs.tpId = 0;
    qs.tpOrderId = 0;

    if (qs.slId) {
      svr.Q_CancelLimit(who, qs.slId);
      qs.slId = 0;
      qs.slOrderId = 0;
    }
  }
}

void DualMAPlugin_onOrderError(QMSServer &svr, const qms_barData &bd,
                               const void *pExtra) {
  if (!pExtra || !bd.ptr) {
    return;
  }
  QuantState &qs = *(QuantState *)bd.ptr;
}

void DualMAPlugin_onLimitPlaced(QMSServer &svr, const qms_barData &bd,
                                const void *pExtra) {
  if (!pExtra || !bd.ptr) {
    return;
  }
  QuantState &qs = *(QuantState *)bd.ptr;

  const qms_limit *pLimit = (const qms_limit *)pExtra;

  if (!pLimit) {
    return;
  }

  const qms_limit &lmt = *pLimit;

  std::stringstream ss;

  ss << "Placed " << lmt.DescribeMode() << " on symbol ";
  if (lmt.pSymbol) {
    const qms_symbol &s = *lmt.pSymbol;
    ss << s.name;
  } else {
    ss << lmt.idSymbol;
  }

  ss << ", Account=";
  if (lmt.pAccount) {
    qms_account &a = *lmt.pAccount;
    ss << a.name << "(" << a.id << ")";
  } else {
    ss << lmt.idAccount;
  }
  ss << ", id=" << lmt.id;
  ss << ", stratId = " << lmt.stratId;
  ss << ", tif=" << lmt.DescribeTIF();
  ss << ", price=" << lmt.price;
  ss << ", stopPx=" << lmt.stopPrice;
  ss << ", volume=" << lmt.volume;
  ss << "\n";

  Log(ss.str().c_str());
}

void DualMAPlugin_onLimitCancelled(QMSServer &svr, const qms_barData &bd,
                                   const void *pExtra) {
  if (!pExtra || !bd.ptr) {
    return;
  }
  QuantState &qs = *(QuantState *)bd.ptr;

  const qms_limit *pLimit = (const qms_limit *)pExtra;

  if (!pLimit) {
    return;
  }

  const qms_limit &lmt = *pLimit;

  std::stringstream ss;

  ss << "Cancelled " << lmt.DescribeMode() << " on symbol ";
  if (lmt.pSymbol) {
    const qms_symbol &s = *lmt.pSymbol;
    ss << s.name;
  } else {
    ss << lmt.idSymbol;
  }

  ss << ", Account=";
  if (lmt.pAccount) {
    qms_account &a = *lmt.pAccount;
    ss << a.name << "(" << a.id << ")";
  } else {
    ss << lmt.idAccount;
  }
  ss << ", id=" << lmt.id;
  ss << ", stratId = " << lmt.stratId;
  ss << ", tif=" << lmt.DescribeTIF();
  ss << ", price=" << lmt.price;
  ss << ", stopPx=" << lmt.stopPrice;
  ss << ", volume=" << lmt.volume;
  ss << "\n";

  Log(ss.str().c_str());
}

void DualMAPlugin_onLimitError(QMSServer &svr, const qms_barData &bd,
                               const void *pExtra) {
  if (!pExtra || !bd.ptr) {
    return;
  }
  QuantState &qs = *(QuantState *)bd.ptr;
}

void DualMAPlugin_onLimitEdited(QMSServer &svr, const qms_barData &bd,
                                const void *pExtra) {
  if (!pExtra || !bd.ptr) {
    return;
  }
  QuantState &qs = *(QuantState *)bd.ptr;

  const qms_limit *pLimit = (const qms_limit *)pExtra;

  if (!pLimit) {
    return;
  }

  const qms_limit &lmt = *pLimit;

  std::stringstream ss;

  ss << "Edited " << lmt.DescribeMode() << " on symbol ";
  if (lmt.pSymbol) {
    const qms_symbol &s = *lmt.pSymbol;
    ss << s.name;
  } else {
    ss << lmt.idSymbol;
  }

  ss << ", Account=";
  if (lmt.pAccount) {
    qms_account &a = *lmt.pAccount;
    ss << a.name << "(" << a.id << ")";
  } else {
    ss << lmt.idAccount;
  }
  ss << ", id=" << lmt.id;
  ss << ", stratId = " << lmt.stratId;
  ss << ", tif=" << lmt.DescribeTIF();
  ss << ", price=" << lmt.price;
  ss << ", stopPx=" << lmt.stopPrice;
  ss << ", volume=" << lmt.volume;
  ss << "\n";

  Log(ss.str().c_str());
}
