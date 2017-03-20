#include "stdafx.h"
//
#include "Globals.h"

#pragma region Globals

const uint32_t g_pluginId = 0x12345; // Use whatever number you want.
HICON g_hIconSmall = 0;
HINSTANCE g_hInstance = 0;
QMSServer *g_pServer = 0;
char g_dllPath[MAX_PATH] = {0};
char g_workDir[MAX_PATH] = {0};
char g_dllName[MAX_PATH] = {0};
char g_dllCfg[MAX_PATH] = {0};
qms_pluginInfo g_pluginInfo = {1, "", "", 0};
volatile long u64_t::nextId = 0;


// Default configuration items.
qms_item g_cfg[eCfgItem_count] = {
	{
		"Orders Per Tick",
		(int)0
	},
	{
		"MS between orders",
		(int)0
	},
	{
		// Distance to place the trade. 
		// 0 = Market order. 
		// > 0 distance in pips for limit. 
		// < 0 invalid.
		"Limit Distance",
		0.0
	},
	{
		// Volume in units.
		"Volume",
		10000.0
	},
};


#pragma endregion

