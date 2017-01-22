#pragma once
/**
 * @file
 */
#include "qms_mm.h"
#include "qms_tarray.h"
#include <stdint.h>

/**
* Bank information.
*/
struct qms_bankInfo
{
	/**
	 * Id of bank.
	 */
	uint16_t id; // 0

	/**
	 * Name of bank.
	 */
	char name[14]; // 0 + 2 = 2
};                 // 2 + 14 = 16

/**
 * Array of banks.
 */
typedef qms_tarray< qms_bankInfo > qms_bankInfoArray;

/**
 * Memory mapped vector of banks.
 */
extern qms::mm::MMVector< qms_bankInfo > qms_bankInfoMM;

/**
 * Address of memory mapped vector of banks.
 */
#define qms_bankInfoMM_path "Local\\qms_bankInfoMM"

/**
 * Find bank from its id.
 * @param id Id of bank.
 * @return Index of bank on success.
 * @return -1 on failure.
 */
int qms_bankInfoMM_find(uint16_t id);
