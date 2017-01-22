#pragma once

/**
* Price and volume.
*/
struct qms_priceVol
{
	/**
	* Price.
	*/
	double price; // 0

	/**
	* Volume.
	*/
	double volume; // 0 + 8 = 8

	/**
	* Time the price was received.
	* Unix time in Micro seconds.
	*/
	uint64_t time; // 8 + 8 = 16

	/**
	* Id of bank.
	* This can be zero if the bank is unknown.
	*/
	uint32_t idBank; // 16 + 8 = 24

	/**
	* Origin of price.
	* This can be zero if the origin is unknown.
	*/
	uint32_t origin; // 24 + 4 = 28

	/**
	* Given an array of prices and values. Calculate the volume weighted
	* average.
	*
	* @param	pv Array of price values.
	* @param	count Number of items in the array.
	* @return	double Weighted average.
	*/
	static double CalcWeightedAverage(const qms_priceVol* pv, int count);
}; // 28 + 4 = 32
