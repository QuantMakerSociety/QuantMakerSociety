#pragma once
/**
 * @file
 */

/**
 * Limit being tracked.
 */
struct qms_limit
{
	/// Limit mode.
	enum eLimitMode
	{
		Limit = 2, ///< Limit.
		Stop  = 3, ///< Stop.
	};

	/// Time in force.
	enum eTimeInForce
	{
		DAY = 0, ///< Good for day.
		GTC = 1, ///< Good until cancel.
		IOC = 2, ///< Immediate or cancel.
		FOK = 3, ///< Fill or kill.
	};

	/**
	* Id of limit.
	*/
	uint32_t id; // 0 + 4 = 4

	/**
	* Id of account.
	*/
	uint32_t idAccount; // 4 + 4 = 8

	/**
	* Id assigned by strategy.
	*/
	uint64_t stratId; // 8 + 8 = 16

	/**
	* Symbol id.
	*/
	uint16_t idSymbol; // 16 + 2 = 18

	/**
	* Bank id.
	*/
	uint16_t idBank; // 18 + 2 = 20

	/**
	* Time in force.
	*/
	uint8_t tif; // 20 + 1= 21

	/**
	* Status
	*/
	uint8_t status; // 21 + 1 = 22

	/**
	* Mode
	*/
	uint8_t mode; // 22 + 1 = 23

	/**
	* For aligment.
	*/
	uint8_t pad8; // 23 + 1 = 24

	/**
	* Volume remaining.
	*/
	double volume; // 24 + 8 = 32

	/**
	* Price
	*/
	double price; // 32 + 8 = 40

	/**
	* Stop price.
	*/
	double stopPrice; // 40 + 8 = 48

	/**
	 * Pointer to account.
	 */
	struct qms_account * pAccount;  // 48 + 8 = 56

	/**
	 * Pointer to symbol
	 */
	struct qms_symbol * pSymbol; // 56 + 8 = 64

	/**
	 * Pointer to quant.
	 */
	struct qms_quant * pQuant; // 64 + 8 - 72

	enum {
		/**
		 * Maximum size for description.
		 */
		MaxDescription = 256 - 72
	};

	/**
	* Description
	*/
	char description[MaxDescription]; // 256

	const char * DescribeMode() const {
		if (mode == Limit)
		{
			if (0 > volume)
			{
				return "SellLimit";
			} else {
				return "BuyLimit";
			}
		}
		
		if(mode == Stop)
		{
			if (0 > volume)
			{
				return "SellStop";
			}
			return "BuyStop";
		}
		return "Unknown";
	}

	const char * DescribeTIF() const
	{
		switch(tif) {
		case DAY: return "DAY";
		case GTC: return "GTC";
		case IOC: return "IOC";
		case FOK: return "FOK";
		}
		return "UKN";
	}

};

static_assert(sizeof(qms_limit) == 256, "sizeof(qms_limit) != 256");

/**
* Array of limits.
*/
typedef qms_tarray< qms_limit > qms_limitArray;

