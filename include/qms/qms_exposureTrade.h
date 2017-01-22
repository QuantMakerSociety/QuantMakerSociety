#pragma once
/**
 * @file
 */
/**
* Helper structure to calculate the exposure on a FIFO mode.
*/
struct qms_exposureTrade
{
	/**
	 * Index of previous trade (relative to st
	 */
	intptr_t prev;

	/**
	 * Index of next trade.
	 */
	intptr_t next;

	/**
	* Id of order.
	*/
	uint32_t id; // 16

	/**
	* Flags for order.
	*/
	uint32_t flags; // 20

	/**
	* Number of units.
	*/
	double vol; // 24

	/**
	* Price per unit.
	*/
	double price; // 32

	/**
	* Convention rate.
	*/
	double toUSD; // 40
};                // 49

/**
* Array of trades in exposure.
*/
typedef qms_tarray< qms_exposureTrade > qms_exposureTradeArray;
