#pragma once
/**
 * @file
 */
#include "qms_item.h"
#include "qms_priceData.h"
struct qms_symbol;

/**
 * Bar data.
 */
struct qms_barData
{
	int32_t             count;         ///< Number of bars in the array.
	int32_t             secondsPerBar; ///< Seconds per bar.
	mutable void*       ptr;           ///< User supplied pointer.
	const qms_symbol*   symbol;        ///< Symbol.
	const qms_account*  acct;          ///< Account.
	const qms_exposure* pExposure;     ///< Exposure for Account/Symbol.
	double*             open;          ///< Array with open prices.
	double*             high;          ///< Array with high prices.
	double*             low;           ///< Array with low prices.
	double*             close;         ///< Array with close prices.
	__time64_t*         time;          ///< Array with bar times.
	qms_priceData       price;         ///< Last known price.
	qms_itemArray       cfg;           ///< Configuration.

	/**
	 * Constructor.
	 */
	qms_barData()
	    : count(0),
	      secondsPerBar(0),
	      ptr(0),
	      symbol(0),
	      acct(0),
	      pExposure(0),
	      open(0),
	      high(0),
	      low(0),
	      close(0),
	      time(0)
	{
	}

	/**
	 * Sets the account.
	 *
	 * @param[in]  pa Pointer to account.
	 */
	void SetAccount(const qms_account* pa)
	{
		acct = pa;
	}
};

/**
 * Bar data message.
 */
enum eBarDataMsg
{
	eBarDataMsg_init,     ///< First time called.
	eBarDataMsg_tick,     ///< Tick received.
	eBarDataMsg_shutdown, ///< End.
};
