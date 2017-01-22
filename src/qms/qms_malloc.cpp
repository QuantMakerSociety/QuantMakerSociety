#include "stdafx.h"
/**
 * @file
 */
//
#include <qms/qms_exception.h>
#include <qms/qms_malloc.h>

void* qms_malloc(size_t siz)
{
	void* p = malloc(siz);
	if (!p) {
		throw QMS_EXCEPTION(__FUNCTION__ " failed.");
	}
	return p;
}
