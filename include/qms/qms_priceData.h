#pragma once

/**
 * @file
 */
struct qms_priceData
{
	double     bid;      ///< Last bid
	double     ask;      ///< Last ask
	double     bidVol;   ///< Last bid volume
	double     askVol;   ///< Last ask volume.
	__time64_t tim;      ///< Last time.
	int64_t    nUpdates; ///< Number of times updated.

	// qms_priceData)() : bid(0), ask(0), bidVol(0), askVol(0), tim(0),
	// nUpdates(0) {}

	void Update(int64_t t, double b, double a, double bv = 0, double av = 0)
	{
		tim    = t;
		bid    = b;
		ask    = a;
		bidVol = bv;
		askVol = av;
		++nUpdates;
	}

	void Reset()
	{
		bid      = 0;
		ask      = 0;
		bidVol   = 0;
		askVol   = 0;
		tim      = 0;
		nUpdates = 0;
	}
};
