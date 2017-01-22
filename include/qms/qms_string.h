#pragma once
/**
 * @file
 */
#include "qms_exception.h"
#include <stdint.h>

/// String.
class qms_string
{
protected:
	int   capacity; ///< Current capacity.
	int   count;    ///< Size of buffer. Including the '\0' at the end.
	char* str;      ///< String.
public:
	/**
	 * Constructor.
	 */
	qms_string() : capacity(0), count(0), str(0)
	{
	}

	/**
	 * Constructor.
	 * @param x What to copy.
	 */
	explicit qms_string(const char* x) : qms_string()
	{
		Set(x);
	}

	/**
	 * Copy constructor.
	 * @param x What to copy.
	 */
	explicit qms_string(const qms_string& x) : qms_string()
	{
		Set(x);
	}

	/**
	 * Destructor.
	 */
	~qms_string()
	{
		Clobber();
	}

	/**
	 * Sets the length and capacity of the string to zero.
	 * Then free the memory of the string.
	 */
	void Clobber();

	/**
	 * Sets the length of the string to zero.
	 */
	void Clear()
	{
		count = 0;
	}

	/**
	 * Reserves space for string.
	 * @param newCapacity New capacity.
	 */
	void Reserve(int newCapacity);

	/**
	 * Copies a string.
	 * @param x String to copy.
	 */
	void Set(const qms_string& x);

	/**
	 * Sets the string based on a C string.
	 * @param x C String.
	 */
	void Set(const char* x);

	/**
	 * Appends a string a the end of this one.
	 * @param x What to append.
	 */
	void Append(const qms_string& x);

	/**
	 * Appends a string at the end of this one.
	 * @param x What to append
	 */
	void Append(const char* x);

	/**
	 * Append an integer at the end of this string.
	 * @param x What to append.
	 */
	void Append(int x);

	/**
	 * Appends a 64 bit integer at the end of this string.
	 * @param x What to append.
	 */
	void Append(int64_t x);

	/**
	 * Appends a 64 bit unsigned integer at the end of this string.
	 * @param x What to string.
	 */
	void Append(uint64_t x);

	/**
	 * Appends a 32 bit unsigned integer at the end of this string
	 * @param x What to append.
	 */
	void Append(uint32_t x);

	/**
	 * Append a 32 bit float at the end of this string
	 * @param x What to append.
	 * @param nDigits Number of digits.
	 */
	void Append(float x, int nDigits = 6);

	/**
	 * Append a 64 bit double at the end of this string
	 * @param x What to append.
	 * @param nDigits Number of digits.
	 */
	void Append(double x, int nDigits = 6);

	/**
	 * Copy operator.
	 * @param x String to copy.
	 * @return *this
	 */
	qms_string& operator=(const qms_string& x)
	{
		Set(x);
		return *this;
	}

	/**
	 * Copy operator.
	 * @param x String to copy.
	 * @return *this
	 */
	qms_string& operator=(const char* x)
	{
		Set(x);
		return *this;
	}

	/**
	 * Access as a string.
	 * @return str
	 */
	explicit operator char*()
	{
		return str;
	}

	/**
	 * Access as a constant string.
	 * @return str or ""
	 */
	explicit operator const char*() const
	{
		return (str) ? str : "";
	}

	/**
	 * Append a string.
	 * @param x String to append.
	 * @return *this
	 */
	qms_string& operator<<(const qms_string& x)
	{
		Append(x);
		return *this;
	}

	/**
	 * Appends a string.
	 * @param x String to append.
	 * @return *this
	 */
	qms_string& operator<<(const char* x)
	{
		Append(x);
		return *this;
	}

	/**
	 * Appends an integer.
	 * @param x Integer to append.
	 * @return *this
	 */
	qms_string& operator<<(const int x)
	{
		Append(x);
		return *this;
	}

	/**
	 * Appends an unsigned integer.
	 * @param x unsigned Integer to append.
	 * @return *this
	 */
	qms_string& operator<<(const uint32_t x)
	{
		Append(x);
		return *this;
	}

	/**
	 * Appends a 64 bit integer.
	 * @param x What to append
	 * @return *this
	 */
	qms_string& operator<<(const int64_t x)
	{
		Append(x);
		return *this;
	}

	/**
	 * Appends a 64 bit unsigned integer.
	 * @param x What to append
	 * @return *this
	 */
	qms_string& operator<<(const uint64_t x)
	{
		Append(x);
		return *this;
	}

	/**
	 * Check if string is empty.
	 * @return true if its empty. false otherwise.
	 */
	bool IsEmpty() const
	{
		return 1 > count;
	}

	/**
	 * Compares this string with another string.
	 * @param x Other string.
	 * @return 0 if *this == x
	 * @return 1 if *this > x
	 * @return -1 if *this < x
	 */
	int Compare(const qms_string& x) const;

	/**
	 * Compares this string with another string.
	 * @param x Other string.
	 * @return 0 if *this == x
	 * @return 1 if *this > x
	 * @return -1 if *this < x
	 */
	int Compare(const char* x) const;

	/**
	 * == Operator
	 * @param x What to compare with.
	 * @return true if *this == x, false otherwise
	 */
	bool operator==(const qms_string& x) const
	{
		return 0 == Compare(x);
	}

	/**
	 * != Operator
	 * @param x What to compare with.
	 * @return true if *this != x, false otherwise
	 */
	bool operator!=(const qms_string& x) const
	{
		return 0 != Compare(x);
	}

	/**
	 * > Operator.
	 * @param x What to compare with.
	 * @return true if *this > x, false otherwise
	 */
	bool operator>(const qms_string& x) const
	{
		return 0 < Compare(x);
	}

	/**
	 * < Operator
	 * @param x What to compare with.
	 * @return true if *this < x, false otherwise
	 */
	bool operator<(const qms_string& x) const
	{
		return 0 > Compare(x);
	}

	/**
	 * >= Operator
	 * @param x What to compare with.
	 * @return true if *this >= x, false otherwise
	 */
	bool operator>=(const qms_string& x) const
	{
		return 0 <= Compare(x);
	}

	/**
	 * Inequality operator.
	 * @param x What to compare with.
	 * @return true if *this <= x, false otherwise
	 */
	bool operator<=(const qms_string& x) const
	{
		return 0 >= Compare(x);
	}
};
