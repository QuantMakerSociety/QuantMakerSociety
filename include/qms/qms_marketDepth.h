#pragma once
/**
 * @file
 */
#include "qms_priceVol.h"

/**
* Market depth.
* A Symbol can receive a price from more than one source.
*/
struct qms_marketDepth
{
	/**
	* Symbol id
	*/
	uint32_t idSymbol; // 0

	/**
	* Bank id
	*/
	uint32_t idBank; // 0 + 4 = 4

	/**
	* Volume weighted average bid.
	*/
	double bidVW; // 4 + 4 = 8

	/**
	* Volume weighted average ask
	*/
	double askVW; // 8 + 8 = 16

	/**
	* Number of bid prices.
	* This could be ZERO if there is no bid price.
	*/
	uint8_t nBid; // 16 + 8 = 24

	/**
	* Number of ask prices.
	* This could be ZERO if there is no ask price.
	*/
	uint8_t nAsk; // 24 + 1 = 25

	/**
	 * Used for padding.
	 */
	uint16_t pad2; // 25 + 1 = 26

	/**
	 * Used for padding.
	 */
	uint32_t pad4; // 26 + 2 = 28

	/**
	 * Bid prices.
	 */
	qms_priceVol bid[QMS_MAX_MD]; // 28 + 4 = 32

	/**
	 * Ask prices.
	 */
	qms_priceVol ask[QMS_MAX_MD]; // 32 + 1,024 = 1,056

	/**
	 * Constructor.
	 * @param ids Id of symbol.
	 * @param idb Id of bank
	 */
	qms_marketDepth(uint32_t ids = 0, uint32_t idb = 0)
	    : idSymbol(ids), idBank(idb)
	{
	}

	/**
	 * Calculates weighted bid and ask.
	 */
	void CalcWeightedBidAndAsk()
	{
		bidVW = qms_priceVol::CalcWeightedAverage(bid, nBid);
		askVW = qms_priceVol::CalcWeightedAverage(ask, nAsk);
	}
}; // 1056 + 1,024 = 2,080 / 16 = 130

/**
 * Memory mapped vector of market depths.
 */
extern qms::mm::MMVector< qms_marketDepth > qms_marketDepthMM;

/**
 * Path to vector of market depths.
 */
#define qms_marketDepthMM_path "Local\\qms_marketDepthMM"

/**
 * Finds market depth
 * @param ids Id of symbol.
 * @param idb Id of bank.
 * @return poitner to market depth if found. 0 if not.
 */
const qms_marketDepth* qms_marketDepthMM_find(uint32_t ids, uint32_t idb);

/**
* Finds index of market depth
* @param ids Id of symbol.
* @param idb Id of bank.
* @return Index of market depth on success.
* @return -(insertion_point + 1) on failure.
*/
int qms_marketDepthMM_findIdx(uint32_t ids, uint32_t idb);
