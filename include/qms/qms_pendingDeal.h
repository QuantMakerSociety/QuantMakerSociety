#pragma once
/**
 * @file
 */
#include <stdint.h>

/**
* Deal in exposure.
*/
struct qms_pendingDeal
{
	int id; ///< Id of deal.
	int idAccount;
	int idSymbol;
	int side;

	int64_t idPlug; ///< Id assigned by plugin 0 = not from plugin. 
	int64_t  openTime; ///< Open time.
	double openPrice; ///< Open price.
	int64_t closeTime; ///< Time of last update.
	double closePrice; ///< Price of last update.
	double conv;       ///< Convention to USD.
	double vol; ///< Volume remaining.
	double profitQ;  ///< Unrealized Profit.
	double profitUSD; ///< Unrealized Profit in US Dollars.
};

typedef qms_tarray< qms_pendingDeal > qms_pendingDealArray;
