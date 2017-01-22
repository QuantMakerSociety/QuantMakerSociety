#pragma once
/**
 * @file
 */
/**
* Deposit or retirement.
*/
struct qms_deposit
{
	/**
	* Id of deposit.
	*/
	uint32_t id; // 0

	/**
	* Id of account.
	*/
	uint32_t idAcct; // 0 + 4 = 4

	/**
	* Id of group.
	*/
	uint32_t idGroup; // 4 + 4 = 8

	/**
	* Id of broker.
	*/
	uint32_t idBroker; // 8 + 4 = 12

	/**
	* Time of deposit.
	* Unix time in seconds.
	*/
	__time64_t time; // 12 + 4 = 16

	/**
	* Amount of deposit.
	*/
	double ammt; // 16 + 8 = 24

	/**
	* Description of deposit.
	* Will be truncated if its length is greater than 95.
	*/
	char desc[96]; // 24 + 8 = 32
};                 // 32 + 96 = 128

/**
* Array of deposits and retirements.
* For use in DLL
*/
typedef qms_tarray<qms_deposit> qms_depositArray;

/**
 * Memory map of deposits.
 * For use in process.
 */
extern qms::mm::MMVector<qms_deposit> qms_depositMM;

/**
 * path to map of deposits.
 */
#define qms_depositMM_path "Local\\qms_depositMM"

/**
 * Finds a deposit.
 * @param id Id of deposit.
 * @return Pointer to deposit if found. NULL if not.
 */
qms_deposit * qms_depositMM_find(uint32_t id);

/**
 * Find index of deposit.
 * @param id Id of deposit.
 * @return Index of deposit. < 0 of not found.
 */
int qms_depositMM_findIdx(uint32_t id);
