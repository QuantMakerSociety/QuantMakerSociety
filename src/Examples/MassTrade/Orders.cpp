#include "stdafx.h"
//
#include "Globals.h"

int PlaceMarket(QMSServer &svr, uint64_t stratId, int idAccount, int idSymbol, double volume, double price)
{
	return svr.PlaceMarketOrder(PluginMain, idAccount, idSymbol, stratId, volume,
		price, "MARKET");
}

int PlaceLimit(QMSServer &svr, uint64_t stratId, int idAccount, int idSymbol, char mode, char tif, double volume, double price, double stopPrice)
{
	return svr.PlaceLimit(PluginMain, idAccount, idSymbol, stratId, mode, tif, volume, price, stopPrice, "LIMIT");
}

int CancelLimit(QMSServer &svr, uint32_t id)
{
	return svr.CancelLimit(id);
}

int EditLimit(QMSServer &svr, uint32_t id, double newPrice, double newStop)
{
	return svr.EditLimit(id, newPrice, newStop);
}
