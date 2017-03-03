#pragma once
/**
 * @file
 */
/**
* Order.
*/
struct qms_order
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
	* Price
	*/
	double price; // 8 + 8 = 16

	/**
	* Volume (Negative for short)
	*/
	double volume; // 16 + 8 = 24

	/**
	* Unix Time in milliseconds.
	*/
	uint64_t mSecTime; // 24 + 8 = 32

	/**
	* Strategy id.
	*/
	uint64_t stratId; // 32 + 8 = 40

	/**
	* Id of symbol.
	*/
	uint32_t idSymbol; // 40 + 4 = 44

	/**
	* Id of bank.
	*/
	uint32_t idBank; // 44 + 4 = 48

	/**
	* Time in force.
	*/
	uint8_t tif; // 48 + 1 = 49

	/**
	* Status
	*/
	uint8_t status; // 49 + 1 = 50

	/**
	* Not currently used
	*/
	uint16_t pad16; // 50 + 2 = 52

	/**
	* Not currently used
	*/
	uint32_t pad32; // 52 + 4 = 56

	/**
	 * Not currently used
	 */
	uint64_t pad64; // 56 + 8 = 64

	/**
	* Description of trade.
	*/
	char description[184]; // 64 + 184 = 256
};
