#pragma once
/**
 * @file
 */
/**
* Exposure.
*/
struct qms_exposure
{
	/**
	* Account id
	*/
	uint32_t idAccount;

	/**
	* Symbol id.
	*/
	uint32_t idSymbol;

	/**
	 * Exposure in base currency.
	 */
	double volume;

	/**
	* Exposure in Quote currency.
	*/
	double exposure;

	/**
	* Exposure in US dollars.
	*/
	double exposureUSD;

	/**
	* Margin used in Quote currency.
	*/
	double margin;

	/**
	* Margin used in USD
	*/
	double marginUSD;
};

/**
 * Array of symbol exposures.
 * For use in DLL
 */
typedef qms_tarray< qms_exposure > qms_exposureArray;

/**
 * Memory map of exposure.
 * for use in process.
 */
extern qms::mm::MMVector< qms_exposure > qms_exposureMM;

/**
 * Path to exposure.
 */
#define qms_exposureMM_path "Local\\qms_exposureMM"
