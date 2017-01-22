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

/**
* Memory mapped array of symbols.
* For usage with external process.
*/
extern qms::mm::MMVector< qms_symbol > qms_symbolMM;

/**
 * Path to symbols.
 */
#define qms_symbolMM_path "Local\\qms_symbolMM"

/**
* Finds a symbol by its id on a symbol array.
* Assumes that the array is sorted by id.
* @param v Symbol array.
* @param ids Id of symbol.
*/
int qms_findSymbolById(const qms_tarray< qms_symbol >& v, const uint32_t ids);
