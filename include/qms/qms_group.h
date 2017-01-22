#pragma once
/**
 * @file
 */
/**
* group
*/
struct qms_group
{
	/**
	* Id
	*/
	uint32_t id; // 0

	/**
	* Id of broker.
	*/
	uint32_t idBroker; // 4

	/**
	* Flags
	*/
	uint32_t flags; // 4 + 4 = 8

	/**
	* Id of bank.
	*/
	uint32_t idBank; // 8 + 4 = 12

	/**
	* Name of group.
	*/
	char name[32]; // 12 + 4 = 16

	/**
	* Leverage
	*/
	double leverage; // 16 + 32 = 48

	/**
	* Percent of usable margin
	*/
	double margin; // 48 + 8 = 56

	/**
	* Commission per million for broker
	*/
	double cpmBroker; // 56 + 8 = 64

	/**
	* CPM agent 1
	*/
	double cpm1; // 64 + 8 = 72

	/**
	* CPM agent 2
	*/
	double cpm2; // 72 + 8 = 80

	/**
	* CPM agent 3
	*/
	double cpm3; // 80 + 8 = 88
};               // 96 + 16 = 112

/**
* Array of groups.
*/
typedef qms_tarray< qms_group > qms_groupArray;

/**
 * Memory mapped file of groups.
 */
extern qms::mm::MMVector< qms_group > qms_groupMM;

/**
 * Find a group in the memory mapped vector.
 * @param id Id of vector.
 * @return Index of group.
 */
extern int qms_groupMM_find(uint32_t id);

/**
 * Path to groups.
 */
#define qms_groupMM_path "Local\\qms_groupMM"
