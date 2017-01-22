#pragma once
#include "qms_assert.h"

/**
 * Malloc that throws an exception if it fails.
 * @param siz Size to alloc.
 * @return Pointer to block of memory.
 */
void* qms_malloc(size_t siz);

/**
 * Malloc for items.
 * @param nItems Number of items > 0
 */
template < typename T > inline T* qms_tmalloc(int nItems)
{
	qms_assert(0 < nItems);
	return (T*)qms_malloc(sizeof(T) * nItems);
}
