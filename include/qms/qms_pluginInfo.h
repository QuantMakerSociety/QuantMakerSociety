#pragma once
/**
 * @file
 */
#include "qms_item.h"

/**
* Information on plugin.
*/
struct qms_pluginInfo
{
	/**
	 * User defined id of plugin.
	 */
	uint64_t id; // 0 : 8

	/**
	 * Name of plugin.
	 */
	char name[56]; // 8 : 64

	/**
	 * Path to log.
	 */
	char logPath[264]; // 64 : 328

	/**
	 * This pointer will be send with each message.
	 */
	void* ctx; // 328 : 336

	/**
	 * Main Configuration. All derivative configurations will come from this.
	 */
	qms_itemArray cfg; // 336 : 344
};                     // 344
