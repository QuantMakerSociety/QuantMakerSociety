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

