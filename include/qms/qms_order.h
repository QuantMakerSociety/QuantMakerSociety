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
	uint32_t id; // 0

	/**
	* Id of account.
	*/
	uint32_t idAcct; // 0 + 4 = 4

	/**
	* Price
	*/
	double price; // 4 + 4 = 8

	/**
	* Volume (Negative for short)
	*/
	double volume; // 8 + 8 = 16

	/**
	* Unix Time in milliseconds.
	*/
	uint64_t mSecTime; // 16 + 8 = 24

	/**
	* Strategy id.
	*/
	uint64_t stratId; // 24 + 8 = 32

	/**
	* Id of symbol.
	*/
	uint32_t idSymbol; // 32 + 8 = 40

	/**
	* Id of bank.
	*/
	uint32_t idBank; // 40 + 4 = 44

	/**
	* Time in force.
	*/
	uint8_t tif; // 44 + 4 = 48

	/**
	* Status
	*/
	uint8_t status; // 48 + 1 = 49

	/**
	* Not currently used
	*/
	uint16_t pad16; // 49 + 1 = 50

	/**
	* Not currently used
	*/
	uint32_t pad32; // 50 + 2 = 52

	/**
	* Not currently used
	*/
	uint64_t pad64; // 52 + 4 = 56

	/**
	* Description of trade.
	*/
	char description[184]; // 56 + 8 = 64
};                         // 64 + 184 = 256
