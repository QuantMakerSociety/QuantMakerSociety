#pragma once
/**
 * @file
 * @brief QMS API main header.
 * @author Quant Maker Society
 */
#ifndef QMS_VER

#ifndef __cplusplus
#error C++ only, for the moment.
#endif

/// Version of the API
#define QMS_VER 20160802

// Windows headers
#include <AccCtrl.h>
#include <AclAPI.h>
#include <WinSock2.h>

// C Headers
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// C++ headers
#include <new>

// Special headers
#include <xmmintrin.h>

// Api headers
#include "qms_assert.h"
#include "qms_constants.h"
#include "qms_cs.h"
#include "qms_itemTree.h"
#include "qms_mm.h"
#include "qms_mm.h"
#include "qms_msg.h"
#include "qms_server.h"
#include "qms_structs.h"
#include "qms_tarray.h"

/**
 * Calculates weighted average.
 * @param value Points to values.
 * @param weight Points to weights.
 * @param count Number of items in both values and weights.
 * @return Weighted average on success. 0 on failure.
 */
double qms_weightedAverage(
    const double* value,
    const double* weight,
    const int     count);

#endif // QMS_VER
