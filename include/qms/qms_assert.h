#pragma once
/**
 * @file
 * @brief Debug Assertions.
 */

#ifdef _DEBUG
#define qms_dbreak() __debugbreak()
#define qms_assert(x)   \
	if (!(x)) {         \
		__debugbreak(); \
	} else {            \
	}
#define qms_verify(x)   \
	if (!(x)) {         \
		__debugbreak(); \
	} else {            \
	}
#else
#define qms_dbreak()
#define qms_assert(x)
#define qms_verify(x) (x)
#endif

/**
 * @def qms_dbreak()
 * On debug mode calls __debugbreak()
 * On release mode does nothing.
 */

/**
 * @def qms_assert(x)
 * On debug mode it sets a debugger breakpoint if an expression is false.
 * On release mode it does nothing
 * @param x Expression.
 */

/**
 * @def qms_verify(x)
 * On debug mode it sets a debugger breakpoint if an expression is false.
 * On release mode process the expression but do not check if its true or false.
 * @param x Expression.
 */
