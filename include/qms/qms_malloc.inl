#include <stdlib.h>
//
#include "qms_exception.h"
#include "qms_malloc.h"

void* qms_malloc(size_t siz)
{
	void* p = malloc(siz);
	if (!p) {
		throw QMS_EXCEPTION(__FUNCTION__ " failed.");
	}
	return p;
}
