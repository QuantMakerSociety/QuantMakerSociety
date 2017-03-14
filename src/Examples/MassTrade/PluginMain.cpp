#include "stdafx.h"
//
#include "Globals.h"

extern "C" {;

__declspec(dllexport) int __stdcall PluginMain(QMSServer &svr, const qms_msg &msg, void *ptr)
{
	switch(msg.msg) {
	case qms_msgAdded::ID: {
		// Plugin was just added.
		qms_msgAdded &m1 = (qms_msgAdded &)msg;
		g_pServer = &svr;
		m1.ptr = 0; // Set the pointer to whatever data you want to save. this will
		// be ptr on next calls.

	} break;

	case qms_msgRemoved::ID: {
		// Plugin was removed.
		const qms_msgRemoved &m1 = (const qms_msgRemoved &)msg;
	} break;

	case qms_getPluginInfo::ID: {
		// Server wants plugin information.
		qms_getPluginInfo &m1 = (qms_getPluginInfo &)msg;
		if(m1.pInfo) {
			memcpy(m1.pInfo, &g_pluginInfo, sizeof(g_pluginInfo));
		}

		svr.RegisterBarDataListener(PluginMain, MassTrade, g_pluginInfo.cfg, 0);

	} break;

	case qms_configChanged::ID: {
		// Configuration changed.
		const qms_configChanged &m1 = (const qms_configChanged &)msg;
	} break;

	case qms_acctCfgChanged::ID: {
		const qms_acctCfgChanged &m1 = (const qms_acctCfgChanged &)msg;
	} break;

	case qms_msgInfo::ID: {
		const qms_msgInfo &m1 = (const qms_msgInfo &)msg;
	} break;

	case qms_msgConnected::ID: {
		// Connected.
	} break;

	case qms_command::ID: {
		const qms_command &m1 = (const qms_command &)msg;
	} break;

	case qms_msgDisconnected::ID: {
	} break;

	case qms_msgQuote::ID: {
		const qms_msgQuote &m1 = (const qms_msgQuote &)msg;
	} break;

	case qms_msgGroupChanged::ID: {
		const qms_msgGroupChanged &m1 = (const qms_msgGroupChanged &)msg;
		const qms_group &g = *m1.group;
	} break;

	case qms_msgAccountChanged::ID: {
		const qms_msgAccountChanged &m1 = (const qms_msgAccountChanged &)msg;
		const qms_account &a = *m1.account;
	} break;

	case qms_msgDealFilled::ID: {
		OnDealFilled(svr, (const qms_msgDealFilled &)msg);
	} break;

	case qms_msgOrderPlaced::ID: {
		const qms_msgOrderPlaced &m1 = (const qms_msgOrderPlaced &)msg;
		const qms_order &o = m1.order;
	} break;

	case qms_msgLimitPlaced::ID: {
		const qms_msgLimitPlaced &m1 = (const qms_msgLimitPlaced &)msg;
		const qms_limit &lmt = m1.limit;
	} break;

	case qms_msgLimitChanged::ID: {
		const qms_msgLimitChanged &m1 = (const qms_msgLimitChanged &)msg;
		const qms_limit &lmt = m1.limit;
	} break;

	case qms_msgLimitCancelled::ID: {
		const qms_msgLimitCancelled &m1 = (const qms_msgLimitCancelled &)msg;
		const qms_limit &lmt = m1.limit;
	} break;

	case qms_msgOrderFailed::ID: {
		const qms_msgOrderFailed &m1 = (const qms_msgOrderFailed &)msg;
		const qms_order &o = m1.order;
	} break;

	case qms_msgLimitFailed::ID: {
		const qms_msgLimitFailed &m1 = (const qms_msgLimitFailed &)msg;
		const qms_limit &lmt = m1.limit;
	} break;

	case qms_msgDeposit::ID: {
		const qms_msgDeposit &m1 = (const qms_msgDeposit &)msg;
		const qms_deposit &dep = m1.deposit;
	} break;

	default: {
		// Unknown message.
	} break;
	}
	return 0;
}

} // extern "C"
