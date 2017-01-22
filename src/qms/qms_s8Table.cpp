#include "stdafx.h"
/**
* @file
*/
//
#include <qms/qms_s8Table.h>

int S8Table::FindIdxInternal(const char* what, H8& ha) const
{
	ha.h = 0;
	if (!tbl || 1 > count) {
		return -1;
	}

	if (what) {
		char*             p   = ha.n;
		const char* const cp1 = &p[7];
		const char*       cp  = what;

		while (*cp && p < cp1) {
			char c = *cp++;
			if (c >= 'a' && c <= 'z') {
				c -= ' '; // To upper case.
			}
			*p++ = c;
		}
		*p = 0;
	}

	const uint64_t x =
	    ha.h; // Make sure that the key is cached during the search.

	int low  = 0;
	int high = count - 1;
	while (low <= high) {
		const int      mid = (low + high) >> 1;
		const uint64_t y   = tbl[mid].x.h;
		if (x > y) {
			low = mid + 1;
			continue;
		}

		if (x < y) {
			high = mid - 1;
			continue;
		}
		return mid;
	}

	return -(low + 1);
}

void S8Table::Clobber()
{
	if (tbl) {
		free(tbl);
		tbl = 0;
	}
	capacity = count = 0;
}

int S8Table::Reserve(int newCapacity)
{
	if (newCapacity <= capacity) {
		return capacity;
	}

	size_t sizeToAlloc = sizeof(Entry) * newCapacity;
	Entry* pe          = (Entry*)malloc(sizeToAlloc);
	if (!pe) {
		return -1;
	}

	if (tbl) {
		if (0 < count) {
			size_t sizeToCopy = sizeof(Entry) * count;
			memcpy(pe, tbl, sizeToCopy);
		}
		free(tbl);
	}
	tbl      = pe;
	capacity = newCapacity;
	return capacity;
}

void* S8Table::GetOrAdd(const char* what, void* value)
{
	H8  ha;
	int idx = FindIdxInternal(what, ha);
	if (0 <= idx) {
		return tbl[idx].value;
	}

	idx = -idx - 1;

	if (capacity == count) {
		if (0 > Reserve(capacity + CapacityIncrementer)) {
			return 0;
		}
	}

	for (int i = count; i > idx; i--) {
		tbl[i] = tbl[i - 1];
	}
	tbl[idx].x.h   = ha.h;
	tbl[idx].value = value;
	return value;
}

int S8Table::Add(const char* what, void* value)
{
	H8  ha;
	int idx = FindIdxInternal(what, ha);
	if (0 <= idx) {
		return -1;
	}

	idx = -idx - 1;

	if (capacity == count) {
		if (0 > Reserve(capacity + CapacityIncrementer)) {
			return -1;
		}
	}

	for (int i = count; i > idx; i--) {
		tbl[i] = tbl[i - 1];
	}
	tbl[idx].x.h   = ha.h;
	tbl[idx].value = value;
	return idx;
}

void* S8Table::Set(const char* what, void* value)
{
	H8  ha;
	int idx = FindIdxInternal(what, ha);
	if (0 <= idx) {
		void* ptr      = tbl[idx].value;
		tbl[idx].value = value;
		return ptr; // Previous value. Replace
	}

	idx = -idx - 1;

	if (capacity == count) {
		if (0 > Reserve(capacity + CapacityIncrementer)) {
			return 0; // NULL = error.
		}
	}

	for (int i = count; i > idx; i--) {
		tbl[i] = tbl[i - 1];
	}
	tbl[idx].x.h   = ha.h;
	tbl[idx].value = value;
	return value;
}
