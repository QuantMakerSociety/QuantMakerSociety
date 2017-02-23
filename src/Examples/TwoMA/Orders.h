#pragma once

/**
* Places a market order.
* @param idAccount Id of account.
* @param idSymbol id of symbol.
* @param volume volume.
* @param price Price
* @return 0 if placed, -1 if not.
*/
int PlaceMarket(QMSServer &svr, uint64_t stratId, int idAccount, int idSymbol,
	double volume, double price);

/**
* Places a limit
* @param idAccount Id of account.
* @param idSymbol id of symbol.
* @param mode
* @param tif
* @param volume volume.
* @param price Price
* @param stopPrice
* @return 0 if placed, -1 if not.
*/
int PlaceLimit(QMSServer &svr, uint64_t stratId, int idAccount, int idSymbol,
	char mode, char tif, double volume, double price,
	double stopPrice);

/**
* Cancel limit.
* @param id Id of limit.
* @return 0 if placed. -1 if not.
*/
int CancelLimit(QMSServer &svr, uint32_t id);

/**
* Edits a limit.
* @param id Id of limit.
* @param newPrice New Price.
* @param newStop New Stop
* @return 0 if placed. -1 if not.
*/
int EditLimit(QMSServer &svr, uint32_t id, double newPrice, double newStop);
