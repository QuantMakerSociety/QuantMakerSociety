#pragma once
/**
 * @file
 */
#include "qms_item.h"
#include "qms_priceData.h"
struct qms_symbol;

/// Mode for bars.
enum eQMSBarMode {
	eQMSBarMode_normal = 0,     ///< Normal open/high/low/close bar.
	eQMSBarMode_kagi,           ///< Kagi bars.
	eQMSBarMode_renko,          ///< Renko bars.
	eQMSBarMode_pointAndFigure, ///< Point and figure bars.
};

struct qms_quant; ///< Quant structure. (PRIVATE)

/**
 * Bar data.
 */
struct qms_barData
{
	qms_quant *         pQuant;        ///< Pointer to internal structure.
	mutable void*       ptr;           ///< User supplied pointer.
	int32_t             count;         ///< Number of bars in the array.
	int32_t             secondsPerBar; ///< Seconds per bar.
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
	int                 barMode;       ///< Mode for bars.
	int                 ida;           ///< Id of account
	int                 ids;           ///< Id of symbol

	/**
	 * Constructor.
	 */
	qms_barData() : 
		pQuant(0),
		ptr(0),
		count(0),
	    secondsPerBar(0),
	    symbol(0),
	    acct(0),
	    pExposure(0),
		barMode(0),
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
		if (pa)
		{
			ida = pa->id;
		}
	}
};

/// Message to a Quant function.
enum eQMSQuantMsg
{
	eQMSQuantMsg_init,             ///< First time called.
	eQMSQuantMsg_tick,             ///< Tick received.
	eQMSQuantMsg_shutdown,         ///< End.
	eQMSQuantMsg_onOrderFilled,    ///< Order filled.
	eQMSQuantMsg_onOrderError,     ///< Order error.
	eQMSQuantMsg_onLimitPlaced,    ///< Limit placed.
	eQMSQuantMsg_onLimitError,     ///< Limit error.
	eQMSQuantMsg_onLimitCancelled, ///< Limit cancelled.
	eQMSQuantMsg_onLimitEdited,    ///< Limit edited.
};
