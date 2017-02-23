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
		// Distance to place the trade. 
		// 0 = Market order. 
		// > 0 distance in pips for limit. 
		// < 0 invalid.
		"Trade Distance",
		0.0
	}, 
	{
		// Distance in pips for stop loss. 0 = No stop loss.
		"Stop Loss", 
		0.0
	}, 
	{
		// Distance in pips for take profit. 0 = No take profit.
		"Take Profit",
		0.0
	}, 
	{
		// Volume in units.
		"Volume", 
		10000.0
	}, 
	{
		// Type of moving average for fast period
		"Fast MA", 
		(int)0,
		qms_item::TP_MA
	}, 
	{
		// Fast period (must be less than slow period)
		"Fast Period", 
		(int)6,
		qms_item::TP_INT
	}, 
	{
		// Type of moving average for slow period
		"Slow MA", 
		(int)0,
		qms_item::TP_MA
	}, 
	{
		// Slow pediod.
		"Slow Period", 
		(int)18, 
		qms_item::TP_INT
	}, 
};


#pragma endregion

