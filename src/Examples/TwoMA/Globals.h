#pragma once
#include <stdint.h>
#include <vector>
#include "QuantState.h"
#include "InitPaths.h"
#include "Log.h"
#include "Orders.h"

/**
 * Enumerator for configuration items.
 */
enum eCfgItem
{
	/**
	 * Trade distance, determines if a limit will be used instead of a market order.
	 */
	eCfgItem_tradeDistance,

	/**
	 * Stop loss.
	 */
	eCfgItem_stopLoss,

	/**
	 * Take profit.
	 */
	eCfgItem_takeProfit,

	/**
	 * Volume in units.
	 */
	eCfgItem_volume,

	/**
	 * Type of fas moving average.
	 */
	eCfgItem_fastMA,

	/**
	 * Period for fast moving average.
	 */
	eCfgItem_fastMAPeriod,

	/**
	 * Type of slow moving average.
	 */
	eCfgItem_slowMA,

	/**
	 * Period for slow moving average.
	 */
	eCfgItem_slowMAPeriod,

	/**
	 * Number of configuration items.
	 */
	eCfgItem_count,
};


#pragma region Globals

/**
 * Configuration.
 */
extern qms_item g_cfg[eCfgItem_count];

/**
 * Plugin id.
 */
extern const uint32_t g_pluginId;

/**
 * Small icon.
 */
extern HICON g_hIconSmall;

/**
 * DLL Instance.
 */
extern HINSTANCE g_hInstance;

/**
 * Server instance.
 */
extern QMSServer *g_pServer;

/**
 * Path to dll.
 */
extern char g_dllPath[MAX_PATH];

/**
 * Working directory.
 */
extern char g_workDir[MAX_PATH];

/**
 * DLL Name.
 */
extern char g_dllName[MAX_PATH];

/**
 * DLL configuration file.
 */
extern char g_dllCfg[MAX_PATH];

/**
 * Plugin information.
 */
extern qms_pluginInfo g_pluginInfo ;

#pragma endregion

/**
 * Creates an uint64_t from two uint32_t
 */
union u64_t
{
	/**
	 * Next id
	 */
	static volatile long nextId;

	/**
	 * Result.
	 */
	uint64_t x64;

	struct
	{
		/**
		 * Low part.
		 */
		uint32_t low32;

		/**
		 * High part.
		 */
		uint32_t high32;
	};

	/**
	 * Constructor.
	 */
	u64_t()
	{
		low32 = (uint32_t)_InterlockedIncrement(&nextId);
		high32 = g_pluginId;
	}

	u64_t& operator ++() {
		low32 = (uint32_t)_InterlockedIncrement(&nextId);
		return *this;
	}

	/**
	 * Copy constructor.
	 * @param x What to copy.
	 */
	u64_t(const u64_t &x) : x64(x.x64) {}

	/**
	 * Constructor.
	 * @param x What to set x64 to
	 */
	u64_t(uint64_t x) : x64(x) {}

	/**
	 * Check if equal.
	 * @paam x What to compare with.
	 * @return true if is equal.
	 */
	bool operator==(const u64_t &x) const { return x64 == x.x64; }
};

#pragma region FunctionDeclarations



extern "C" {;


/**
* Plugin listener.
* @param svr Server.
* @param msg Message received.
* @param ptr Pointer with context.
* @return 0 on success.
*/
__declspec(dllexport) int __stdcall PluginMain(QMSServer &svr, const qms_msg &msg, void *ptr);

/**
* Get plugin 16x16 icon.
* @return Handle of icon.
*/
__declspec(dllexport) HICON __stdcall PluginGetIcon();

} // extern "C"
#pragma endregion

#include "DualMAPlugin.h"
#include "OnDealFilled.h"
