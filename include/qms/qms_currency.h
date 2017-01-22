#pragma once
/**
 * @file
 */

/**
 * Currency.
 */
struct qms_currency
{
	/**
	 * Id of currency.
	 */
	uint32_t id; // 0

	/**
	 * Name of currency.
	 */
	char name[28]; // 4

	/**
	 * Used to format a value on the currency
	 * Default = "%.6f"
	 */
	char fmtValue[16]; // 32

	/**
	 * Used to format the volume of a currency.
	 * Default = "%.0f"
	 */
	char fmtVol[16]; // 48
};                   // 64

/**
 * Array of currencies.
 */
typedef qms_tarray< qms_currency > qms_currencyArray;

/**
 * Memory mapped vector of currencies.
 */
extern qms::mm::MMVector< qms_currency > qms_currencyMM;

/**
 * Path to currency memory mapped file.
 */
#define qms_currencyMM_path "Local\\qms_currencyMM"
