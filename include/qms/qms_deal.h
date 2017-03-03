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
	uint32_t id; // 0 + 4 = 4

	/**
	 * Id of account.
	 */
	uint32_t idAcct; // 4 + 4 = 8

	/**
	 * Id of symbol.
	 */
	uint32_t idSymbol; // 8 + 4 = 12

	/**
	 * Id of bank.
	 */
	uint32_t idBank; // 12 + 4 = 16

	/**
	 * Time of order.
	 * Unix time in milliSeconds
	 */
	uint64_t mSecTime; // 16 + 8 = 24

	/**
	 * Volume
	 */
	double volume; // 24 + 8 = 32

	/**
	 * Price
	 */
	double price; // 32 + 8 = 40

	/**
	 * Conversion rate at the moment of the trade.
	 */
	double conv; // 40 + 8 = 48

	/**
	 * Id of strategy.
	 */
	uint64_t stratId; // 48 + 8 = 56

	/**
	 * Pointer to symbol. 
	 */
	const struct qms_symbol * pSymbol; // 56 + 8 = 64

	/**
	 * Pointer to account.
	 */
	const struct qms_account * pAccount; // 64 + 8 = 72

	/**
	 * Pointer to exposure.
	 */
	const struct qms_exposure * pExposure; // 72 + 8 = 80

	/**
	 * Pointer to Quant. 0 if none.
	 */
	const struct qms_quant * pQuant; // 80 + 8 = 88

	enum {
		DescSize = 256 - 88 ///< Size of description buffer.
	};

	/**
	 * Description
	 */
	char description[DescSize]; // 256
};

static_assert(sizeof(qms_deal) == 256, "sizeof(qms_deal) == 256");

/**
* Array of deals.
* For use in a dll.
*/
typedef qms_tarray< qms_deal > qms_dealArray;
