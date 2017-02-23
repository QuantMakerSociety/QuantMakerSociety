#pragma once
/**
* @file
*/

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN
#endif

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

// Windows headers.

#include <Windows.h>

// C headers
#include <share.h>
#include <stdarg.h>
#include <math.h>

// c++ headers
#include <vector>
#include <string>
#include <sstream>
#include <strstream>

// gbxapi
#include <qms/qms.h>

// talib
#include <qms_ta64/qms_ta64.h>

#pragma intrinsic (_InterlockedIncrement, _InterlockedDecrement)

