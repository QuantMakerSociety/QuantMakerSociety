#include <math.h>
#include <string.h>

#include "ta_func.h"

#ifndef TA_UTILITY_H
#include "ta_utility.h"
#endif

#ifndef TA_MEMORY_H
#include "ta_memory.h"
#endif

#define TA_PREFIX(x) TA_##x
#define INPUT_TYPE double

TA_LIB_API int TA_JRSX_Lookback(int optInTimePeriod) {
  if ((int)optInTimePeriod == TA_INTEGER_DEFAULT) {
    optInTimePeriod = 30;
  } else if (((int)optInTimePeriod < 2) || ((int)optInTimePeriod > 100000)) {
    return -1;
  }

  return optInTimePeriod - 1;
}

TA_LIB_API TA_RetCode TA_JRSX(int startIdx, int endIdx, const double inReal[],
                              int optInTimePeriod, int *outBegIdx,
                              int *outNBElement, double outReal[]) {
  if (startIdx < 0) {
    return TA_OUT_OF_RANGE_START_INDEX;
  }

  if ((endIdx < 0) || (endIdx < startIdx)) {
    return TA_OUT_OF_RANGE_END_INDEX;
  }

  if (!inReal) {
    return TA_BAD_PARAM;
  }

  if ((int)optInTimePeriod == TA_INTEGER_DEFAULT) {
    optInTimePeriod = 30;
  } else if (((int)optInTimePeriod < 2) || ((int)optInTimePeriod > 100000)) {
    return TA_BAD_PARAM;
  }
  if (!outReal) {
    return TA_BAD_PARAM;
  }

  int lookbackTotal = (optInTimePeriod - 1);

  if (startIdx < lookbackTotal) {
    startIdx = lookbackTotal;
  }

  if (startIdx > endIdx) {
    *outBegIdx = 0;
    *outNBElement = 0;
    return TA_SUCCESS;
  }

  double periodTotal = 0;
  int trailingIdx = startIdx - lookbackTotal;

  int i = trailingIdx;
  if (optInTimePeriod > 1) {
    while (i < startIdx) {
      periodTotal += inReal[i++];
    }
  }

  int outIdx = 0;
  do {
    periodTotal += inReal[i++];
    double tempReal = periodTotal;
    periodTotal -= inReal[trailingIdx++];
    outReal[outIdx++] = tempReal / optInTimePeriod;
  } while (i <= endIdx);

  *outNBElement = outIdx;
  *outBegIdx = startIdx;
  return TA_SUCCESS;
}
