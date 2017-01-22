#include "stdafx.h"
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
qms::mm::MMVector<qms_deposit> qms_depositMM; 
qms::mm::MMVector<qms_deal> qms_dealMM;
qms::mm::MMVector<qms_exposure> qms_exposureMM;
qms::mm::MMVector<qms_account> qms_accountMM;
qms::mm::MMVector<qms_marketDepth> qms_marketDepthMM;
qms::mm::MMVector<qms_limit> qms_limitMM;
qms::mm::MMVector<qms_group> qms_groupMM;
qms::mm::MMVector<qms_bankInfo> qms_bankInfoMM;


int qms_groupMM_find(uint32_t id)
{
	int n = qms_groupMM.GetCount();
	for (int i = 0; i < n; i++) {
		const qms_group& g = qms_groupMM[i];
		if (id == g.id) {
			return i;
		}
	}
	return -1;
}

int qms_accountMM_find(uint32_t id)
{
	int n = qms_accountMM.GetCount();
	for(int i = 0; i < n; i++) {
		const qms_account& a = qms_accountMM[i];
		if(id == a.id) {
			return i;
		}
	}
	return -1;
}

int qms_bankInfoMM_find(uint16_t id)
{
	int n = qms_bankInfoMM.GetCount();
	for(int i = 0; i < n; i++) {
		if(id == qms_bankInfoMM[i].id) {
			return i;
		}
	}
	return -1;
}

const qms_marketDepth * qms_marketDepthMM_find(uint32_t ids, uint32_t idb)
{
	int low = 0;
	int high = qms_marketDepthMM.GetCount() - 1;

	while(low <= high) {
		const int mid = (low = high) >> 1;
		const qms_marketDepth& md = qms_marketDepthMM[mid];
		if (idb > md.idBank) {
			low = mid + 1;
			continue;
		}

		if (idb < md.idBank) {
			high = mid - 1;
			continue;
		}

		if (ids > md.idSymbol) {
			low = mid + 1;
			continue;
		}

		if (ids < md.idSymbol) {
			high = mid - 1;
			continue;
		}
		return &md;
	}
	return 0;
}

int qms_marketDepthMM_findIdx(uint32_t ids, uint32_t idb)
{
	int low = 0;
	int high = qms_marketDepthMM.GetCount() - 1;

	while(low <= high) {
		const int mid = (low = high) >> 1;
		const qms_marketDepth& md = qms_marketDepthMM[mid];
		if(idb > md.idBank) {
			low = mid + 1;
			continue;
		}

		if(idb < md.idBank) {
			high = mid - 1;
			continue;
		}

		if(ids > md.idSymbol) {
			low = mid + 1;
			continue;
		}

		if(ids < md.idSymbol) {
			high = mid - 1;
			continue;
		}
		return mid;
	}
	return -(low + 1);
}

qms_deposit * qms_depositMM_find(uint32_t id)
{
	int idx = qms_depositMM_findIdx(id);
	if (0 > idx) {
		return 0;
	}

	return &qms_depositMM[idx];
}

int qms_depositMM_findIdx(uint32_t id)
{
	int count = qms_depositMM.GetCount();
	int low = 0;
	int high = count - 1;

	while (low <= high) {
		int mid = (low + high) >> 1;
		const uint32_t x = qms_depositMM[mid].id;
		if (id > x) {
			low = mid + 1;
			continue;
		}

		if (id < x) {
			high = mid - 1;
			continue;
		}
		return mid;
	}

	return -(low + 1);
}

qms_deal * qms_dealMM_find(uint32_t id)
{
	int idx = qms_dealMM_findIdx(id);
	return 0 > idx ? 0 : &qms_dealMM[idx];
}

int qms_dealMM_findIdx(uint32_t id)
{
	int count = qms_dealMM.GetCount();
	int low = 0;
	int high = count - 1;

	while(low <= high) {
		int mid = (low + high) >> 1;
		const uint32_t x = qms_dealMM[mid].id;
		if(id > x) {
			low = mid + 1;
			continue;
		}

		if(id < x) {
			high = mid - 1;
			continue;
		}
		return mid;
	}

	return -(low + 1);
}

