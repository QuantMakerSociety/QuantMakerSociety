#include "stdafx.h"
//
#include "Globals.h"

void OnDealFilled(QMSServer &svr, const qms_msgDealFilled &msg)
{
	const qms_deal &d = msg.deal;

	u64_t x(d.stratId);

	if(g_pluginId != x.high32) {
		return; // Not ours. Ignore it.
	}

	uint32_t internalId = x.low32;

	if (!d.pSymbol)
	{
		Log("DEAL %d : %I64u : %d : vol = %f, price = %f, desc=\"%s\"\n", 
			d.id, 
			d.stratId,
			d.idSymbol, 
			d.volume, 
			d.price, 
			d.description);
	} else {
		Log("DEAL %d : %I64u : %s : vol = %f price = %f, desc=\"%s\"\n",
			d.id, 
			d.stratId,
			d.pSymbol->name,
			d.volume, 
			d.price, 
			d.description);
	}
}
