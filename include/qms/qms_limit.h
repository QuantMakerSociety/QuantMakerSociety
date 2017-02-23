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
	uint32_t id; // 0

	/**
	* Id of account.
	*/
	uint32_t idAccount; // 0 + 4 = 4

	/**
	* Id assigned by strategy.
	*/
	uint64_t stratId; // 4 + 4 = 8

	/**
	* Symbol id.
	*/
	uint16_t idSymbol; // 8 + 8 = 16

	/**
	* Bank id.
	*/
	uint16_t idBank; // 16 + 2 = 18

	/**
	* Time in force.
	*/
	uint8_t tif; // 18 + 2 = 20

	/**
	* Status
	*/
	uint8_t status; // 20 + 1 = 21

	/**
	* Mode
	*/
	uint8_t mode; // 21 + 1 = 22

	/**
	* For aligment.
	*/
	uint16_t pad8; // 22 + 1 = 23

	/**
	* Volume remaining.
	*/
	double volume; // 23 + 1 = 24

	/**
	* Price
	*/
	double price; // 24 + 8 = 32

	/**
	* Stop price.
	*/
	double stopPrice; // 32 + 8 = 40

	/**
	* Description
	*/
	char description[128]; // 40 + 8 = 48
};                         // 48 + 176

/**
* Array of limits.
*/
typedef qms_tarray< qms_limit > qms_limitArray;

