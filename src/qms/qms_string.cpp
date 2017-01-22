#include "stdafx.h"
/**
 * @file
 */
#include <qms/qms_malloc.h>
#include <qms/qms_string.h>

void qms_string::Clobber()
{
	if (str) {
		free(str);
		str = 0;
	}
	capacity = 0;
	count    = 0;
}

void qms_string::Reserve(int newCapacity)
{
	if (capacity < newCapacity) {
		char* p = qms_tmalloc< char >(newCapacity);
		if (str) {
			if (0 < count) {
				memcpy(p, str, count + 1);
			} else {
				p[0] = 0;
			}
			free(str);
		}
		str      = p;
		capacity = newCapacity;
	}
}

void qms_string::Set(const qms_string& x)
{
	if (this == &x) {
		return;
	}

	if (1 > x.count) {
		count = 0;
		return;
	}

	int minCapacity = x.count + 1;

	if (capacity < minCapacity) {
		Reserve(minCapacity);
	}
	memcpy(str, x.str, minCapacity);
	count = x.count;
}

void qms_string::Set(const char* x)
{
	if (!x) {
		count = 0;
		return;
	}

	if (str == x) {
		return;
	}

	int len = (int)strlen(x);
	if (1 > len) {
		count = 0;
		return;
	}

	int minCapacity = len + 1;
	if (minCapacity > capacity) {
		Reserve(minCapacity);
	}

	memcpy(str, x, minCapacity);
	count = len;
}

void qms_string::Append(const qms_string& x)
{
	if (this == &x) {
		Append(qms_string(x));
		return;
	}

	if (1 > x.count) {
		return;
	}

	int minCapacity = count + x.count + 1;

	if (capacity < minCapacity) {
		Reserve(minCapacity);
	}

	memcpy(str + count, x.str, x.count + 1);
	count += x.count;
}

void qms_string::Append(const char* x)
{
	if (!x) {
		return;
	}

	if (str == x) {
		Append(qms_string(x));
		return;
	}

	int len = (int)strlen(x);
	if (1 > len) {
		return;
	}

	int minCapacity = count + len + 1;
	if (minCapacity > capacity) {
		Reserve(minCapacity);
	}

	memcpy(str + count, x, len + 1);
	count += len;
}

void qms_string::Append(int x)
{
	char sz[64];
	Append(_itoa(x, sz, 10));
}

void qms_string::Append(int64_t x)
{
	char sz[64];
	Append(_i64toa(x, sz, 10));
}

void qms_string::Append(uint64_t x)
{
	char sz[64];
	Append(_ui64toa(x, sz, 10));
}

void qms_string::Append(uint32_t x)
{
	char sz[64];
	Append(_ui64toa(x, sz, 10));
}

void qms_string::Append(float x, int nDigits)
{
	char sz[64];
	if (_gcvt_s(sz, sizeof(sz), x, nDigits)) {
		return;
	}
	Append(sz);
}

void qms_string::Append(double x, int nDigits)
{
	char sz[64];
	if (_gcvt_s(sz, sizeof(sz), x, nDigits)) {
		return;
	}
	Append(sz);
}

int qms_string::Compare(const qms_string& x) const
{
	if (this == &x) {
		return 0;
	}

	if (0 == count) {
		if (0 == x.count) {
			return 0;
		}
		return -1;
	}

	if (0 == x.count) {
		return 1;
	}

	return strcmp(str, x.str);
}

int qms_string::Compare(const char* x) const
{
	if (str == x) {
		return 0;
	}

	if (0 == x) {
		if (0 == count) {
			return 0;
		}
		return 1;
	}

	int len = (int)strlen(x);
	if (0 < len) {
		if (0 == count) {
			return -1;
		}
		return strcmp(str, x);
	}

	if (0 == count) {
		return 0;
	}
	return 1;
}
