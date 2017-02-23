#pragma once
/**
 * @file
 */
/**
 * Symbol.
 * For our purpose a symbol is a pair like EUR/USD. Except that the name would
 * be EURUSD.
 */
struct qms_symbol
{
	/**
	 * Symbol id.
	 */
	uint32_t id; // 0

	/**
	 * Number of digits.
	 */
	uint8_t nDigits; // 0 + 4 = 4

	/**
	 * Id of base currency.
	 */
	uint32_t base; // 4 + 4 = 8

	/**
	 * Id of quote currency.
	 */
	uint32_t quote; // 8 + 4 = 12

	/**
	 * Symbol name.
	 */
	char name[16]; // 12 + 4 = 16
};                 // 16 + 16 = 32

/**
* Array of symbols.
*/
typedef qms_tarray< qms_symbol > qms_symbolArray;
