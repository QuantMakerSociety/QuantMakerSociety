#pragma once
#include <stdint.h>
#include <vector>

// State for plugin
struct QuantState
{
	int lastSignal; // Last signal received.
	int ids;           // Id of symbol.
	int ida;           // Id of account.
	int secondsPerBar; // Seconds per bar.
	int ordersPerTick; ////< Order per tick.

	char useLimits;     // Set to 1 to use limits instead of market.
	double td;
	double volume;    // Volume to trade.

	uint32_t dealId;    // Deal id
	uint64_t waitingId; // Waiting id.

	double volPlaced; // Volume placed.
	double openPrice; // Open price.

	const qms_account *pAccount;   // Pointer to account.
	
	const qms_symbol *pSymbol;     // Pointer to symbol.
	
	const qms_exposure *pExposure; // Pointer to exposure

	uint32_t slId; ///< GBX order id for stop loss. 

	uint32_t tpId; ///< GBX order id for take profit.

	uint64_t slOrderId; ///< Our order id for stop loss.

	uint64_t tpOrderId; ///< Our order id for take profit.

	int slSide; ///< Side of stoploss.

	double slLastUpdatePrice; ///< Last stop loss update price.

	double slPips; ///< Delta pips of stop loss.

	double outFast[1024]; /// Buffer for talib to compute the fast moving average.

	double outSlow[1024]; ///<  Buffer for talib to compute the low moving average.

	QuantState();

	QuantState(const QuantState &x);

	QuantState &operator=(const QuantState &x);
};

extern std::vector<QuantState *> g_qs;

QuantState * NewQuantState();

void FreeQuantState(QuantState *pl);
