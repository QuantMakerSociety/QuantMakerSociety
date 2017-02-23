#include "stdafx.h"
/**
 * @file
 */
#include <qms/qms.h>

double qms_weightedAverage(const double* value, const double* weight, const int count)
{
	double accum = 0;
	double totalWeight = 0;

	for(int i = 0; i < count; i++) {
		const double w = weight[i];
		accum += w * value[i];
		totalWeight += w;
	}

	return (1E-6 > totalWeight) ? 0 : accum / totalWeight;
}

int qms_findSymbolById(const qms_tarray<qms_symbol>& v, const uint32_t ids)
{
	if(1 > v.count) {
		return -1;
	}

	int low = 0;
	int high = v.count - 1;
	while(low <= high) {
		int mid = (low + high) >> 1;
		const qms_symbol& x = v[mid];
		if(ids > x.id) {
			low = mid + 1;
			continue;
		}

		if(ids < x.id) {
			high = mid - 1;
			continue;
		}
		return mid;
	}
	return -(low + 1);
}

double qms_priceVol::CalcWeightedAverage(const qms_priceVol* pv, int count)
{
	if(!pv || 1 > count) {
		return 0.0;
	}

	double accum = pv[0].price;
	if(1 == count) {
		return accum;
	}

	double totalWeight = pv[0].volume;
	accum *= totalWeight;

	for(int i = 1; i < count; i++) {
		const double w = pv[i].volume;
		accum += w * pv[i].price;
		totalWeight += w;
	}

	return (1E-7 > totalWeight) ? 0.0 : accum / totalWeight;
}

qms_msgInfo* qms_msgInfo::Create(void* buff, size_t buffLen, uint8_t mode, const char* txt)
{
	if(!buff || buffLen < sizeof(qms_msgInfo)) {
		return 0;
	}

	size_t lenStr = strlen(txt);

	if(lenStr > 255) {
		lenStr = 255; // Truncate.
	}

	if(buffLen < sizeof(qms_msgInfo) + lenStr) {
		return 0;
	}

	qms_msgInfo* p = (qms_msgInfo*)buff;

	p->msg = ID;
	p->flg = mode;
	p->len = (uint16_t)(sizeof(qms_msgInfo) + lenStr);
	if(0 < lenStr) {
		memcpy(p->message, txt, lenStr);
	}

	p->message[lenStr] = 0;
	return p;
}

qms::mm::MMVector<qms_currency> qms_currencyMM;
qms::mm::MMVector<qms_symbol> qms_symbolMM;



