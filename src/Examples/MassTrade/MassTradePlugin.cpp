#include "stdafx.h"
//
#include "Globals.h"

void MassTrade_onTick(QMSServer &svr, const qms_barData &bd);

void MassTrade_onInit(QMSServer &svr, const qms_barData &bd);

void MassTrade_onShutdown(QMSServer &svr, const qms_barData &bd);

void MassTrade_onOrderFilled(QMSServer &svr, const qms_barData &bd, const void *pExtra);

void MassTrade_onOrderError(QMSServer &svr, const qms_barData &bd, const void *pExtra);

void MassTrade_onLimitPlaced(QMSServer &svr, const qms_barData &bd, const void *pExtra);

void MassTrade_onLimitCancelled(QMSServer &svr, const qms_barData &bd, const void *pExtra);

void MassTrade_onLimitError(QMSServer &svr, const qms_barData &bd, const void *pExtra);

void MassTrade_onLimitEdited(QMSServer &svr, const qms_barData &bd, const void *pExtra);

void MassTrade(QMSServer &svr, int msg, const qms_barData &bd, const void *pExtra)
{
  switch (msg) {
  case eQMSQuantMsg_tick:
    return MassTrade_onTick(svr, bd);

  case eQMSQuantMsg_init:
    return MassTrade_onInit(svr, bd);

  case eQMSQuantMsg_shutdown:
    return MassTrade_onShutdown(svr, bd);

  case eQMSQuantMsg_onOrderFilled:
    return MassTrade_onOrderFilled(svr, bd, pExtra);

  case eQMSQuantMsg_onOrderError:
    return MassTrade_onOrderError(svr, bd, pExtra);

  case eQMSQuantMsg_onLimitPlaced:
    return MassTrade_onLimitPlaced(svr, bd, pExtra);

  case eQMSQuantMsg_onLimitError:
    return MassTrade_onLimitError(svr, bd, pExtra);

  case eQMSQuantMsg_onLimitCancelled:
    return MassTrade_onLimitCancelled(svr, bd, pExtra);

  case eQMSQuantMsg_onLimitEdited:
    return MassTrade_onLimitEdited(svr, bd, pExtra);

  default:
    return;
  }
}

void MassTrade_onTick(QMSServer &svr, const qms_barData &bd)
{
  if (!bd.pQuant) {
    return;
  }

  qms_quant &who = *bd.pQuant;

  if (!bd.ptr || !bd.acct || !bd.symbol || !bd.pExposure) {
    svr.Q_UpdateStatusMsg(who,
                          "\2"
                          "\xc"
                          "INVALID PARAMETERS");
    return;
  }

  qms_assert(0 < bd.price.tim);

  std::stringstream ss;

  QuantState &        qs = *(QuantState *)bd.ptr;
  const qms_account & a  = *bd.acct;
  const qms_symbol &  s  = *bd.symbol;
  const qms_exposure &e  = *bd.pExposure;

  double bid = bd.price.bid;
  double ask = bd.price.ask;

  ss << s.name << ": bid=" << bd.price.bid << ", ask=" << bd.price.ask << "\n";
  ss << "Account " << a.name << "(" << a.id << ") : Balance = " << a.balance << ", Equity = " << a.equity << "\n";
  ss << "Exposure vol = " << e.volume << ", Q= " << e.exposure << ", USD = " << e.exposureUSD << ", M=" << e.margin << ", MUSD = " << e.marginUSD << "\n";

  qs.pAccount = bd.acct;
  qs.pSymbol  = bd.symbol;

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
  double td     = cfg[eCfgItem_tradeDistance].val.dbl;

  if (1E-6 > td) {
    td = 0;
  }

  if (1E-6 < fabs(td - qs.td)) {
    qs.td        = td;
    qs.useLimits = (1E-6 < td);
  }

  if (1E-6 < fabs(volume - qs.volume)) {
    qs.volume = volume;
  }

  if (1E-6 < volume) {
    u64_t  oid;
    double vol    = volume;
    double negVol = -volume;

    qms_quant &who = *bd.pQuant;
    if (qs.useLimits) {
      static const double digitsToPips[] = {
          1.0, 0.1, 0.01, 0.001, 0.0001, 0.0001, 0.0001,
      };
      double onePip   = digitsToPips[bd.symbol->nDigits];
      double pips     = qs.td * onePip;
      double pipAdder = qs.td * onePip;
      pips            = pipAdder;

      for (int i = 0; i < 10; ++i) {
        svr.Q_PlaceLimit(who, ida, ids, oid.x64, 2, 0, vol, bd.price.ask - pips, 0, "BUY_LIMIT");
        svr.Q_PlaceLimit(who, ida, ids, oid.x64, 2, 0, negVol, bd.price.bid + pips, 0, "SELL_LIMIT");
        pips += pipAdder;
      }
    } else {
      for (int i = 0; i < 10; ++i) {
        (void)svr.Q_PlaceMarketOrder(who, ida, ids, oid.x64, vol, bd.price.ask, "BUY_MARKET");
        (void)svr.Q_PlaceMarketOrder(who, ida, ids, oid.x64, negVol, bd.price.bid, "SELL_MARKET");
      }
    }
    svr.Q_UpdateStatusMsg(who, ss.str().c_str());
    Log("%s\n", ss.str().c_str());
  }
}

void MassTrade_onInit(QMSServer &svr, const qms_barData &bd)
{
  bd.ptr                   = NewQuantState();
  const qms_itemArray &cfg = bd.cfg;
  std::stringstream    ss;

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

void MassTrade_onShutdown(QMSServer &svr, const qms_barData &bd)
{
  FreeQuantState((QuantState *)bd.ptr);
  bd.ptr = 0;
}

void MassTrade_onOrderFilled(QMSServer &svr, const qms_barData &bd, const void *pExtra)
{
  if (!pExtra || !bd.ptr) {
    return;
  }

  QuantState &    qs  = *(QuantState *)bd.ptr;
  const qms_deal &d   = *(const qms_deal *)pExtra;
  qms_quant &     who = *bd.pQuant;
  Log("Filled order %d in account %d, sbol=%d, price=%f, strat=%I64u\n", d.id, d.idAcct, d.idSymbol, d.price, d.stratId);
  Log("qs.slOrderId = %I64u, slId = %u\n", qs.slOrderId, qs.slId);
  Log("qs.tpOrderId = %I64u, tpId = %u\n", qs.tpOrderId, qs.slId);
  Log("Desc=\"%s\"\n", d.description);
}

void MassTrade_onOrderError(QMSServer &svr, const qms_barData &bd, const void *pExtra)
{
  if (!pExtra || !bd.ptr) {
    return;
  }
  QuantState &qs = *(QuantState *)bd.ptr;
}

void MassTrade_onLimitPlaced(QMSServer &svr, const qms_barData &bd, const void *pExtra)
{
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

void MassTrade_onLimitCancelled(QMSServer &svr, const qms_barData &bd, const void *pExtra)
{
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

void MassTrade_onLimitError(QMSServer &svr, const qms_barData &bd, const void *pExtra)
{
  if (!pExtra || !bd.ptr) {
    return;
  }
  QuantState &qs = *(QuantState *)bd.ptr;
}

void MassTrade_onLimitEdited(QMSServer &svr, const qms_barData &bd, const void *pExtra)
{
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
