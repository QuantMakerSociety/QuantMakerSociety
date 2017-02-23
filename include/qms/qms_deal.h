#pragma once
/**
 * @file
 */
/**
 * Order dealed.
 */
struct qms_deal
{
	/**
	 * Id of order.
	 */
	uint32_t id; // 0

	/**
	 * Id of account.
	 */
	uint32_t idAcct; // 0 + 4 = 4

	/**
	 * Id of symbol.
	 */
	uint32_t idSymbol; // 4 + 4 = 8

	/**
	 * Id of bank.
	 */
	uint32_t idBank; // 8 + 4 = 12

	/**
	 * Time of order.
	 * Unix time in milliSeconds
	 */
	uint64_t mSecTime; // 12 + 4 = 16

	/**
	 * Volume
	 */
	double volume; // 16 + 8 = 24

	/**
	 * Price
	 */
	double price; // 24 + 8 = 32

	/**
	 * Conversion rate at the moment of the trade.
	 */
	double conv; // 32 + 8 = 40

	/**
	 * Id of strategy.
	 */
	uint64_t stratId; // 40 + 8 = 48

	/**
	 * Description
	 */
	char description[184]; // 48 + 8 = 56
};                         // 72 + 184 = 256

/**
* Array of deals.
* For use in a dll.
*/
typedef qms_tarray< qms_deal > qms_dealArray;
