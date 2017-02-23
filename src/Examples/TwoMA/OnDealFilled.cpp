#include "stdafx.h"
//
#include "Globals.h"

void OnDealFilled(QMSServer &svr, const qms_msgDealFilled &msg)
{
	const qms_deal &d = msg.deal;

	u64_t x(d.stratId);

	if(g_pluginId != x.high32) {
		return; // Not ours.
	}

	// Is ours.
	Log("Deal %d : %d : vol = %f price = %f\n", d.id, d.idSymbol, d.volume, d.price);

	int n = (int)g_qs.size();
	for(int i = 0; i < n; i++) {
		QuantState &p = *g_qs[i];
		if(p.waitingId == x.x64) {
			p.dealId = d.id;
			p.volPlaced = d.volume;

			if(p.pSymbol) {
				const qms_symbol &s = *p.pSymbol;
				double vol = p.volume;
				static const double digitsToPips[] = {
					1.0, 0.1, 0.01, 0.001, 0.0001, 0.0001, 0.0001,
				};
				double onePip = digitsToPips[s.nDigits];

				u64_t tpU64;
				u64_t slU64;
				break;
			}
		}
	}
}
