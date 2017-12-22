#pragma once
/**
 * @file
 */
#ifdef TA_LIB_SHARED
#undef TA_LIB_SHARED
#endif

#ifndef TA_LIB_API
#define TA_LIB_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include "ta_libc.h"

#ifdef __cplusplus
} // extern "C"
#endif

/**
 * @def TA_LIB_SHARED
 * Makes sure that the library sets its flags to shared.
 */

/**
 * @def TA_LIB_API
 * Declares calling convertion for library
 */