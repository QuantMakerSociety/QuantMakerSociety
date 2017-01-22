#pragma once
/**
 * @file
 */
#include <stdint.h>

/**
 * Search table for strings less than 8 bytes.
 */
class S8Table
{
protected:
	/**
	 * Union with name and hash.
	 */
	union H8
	{
		/**
		 * Hash
		 */
		uint64_t h; // 0

		/**
		 * Name
		 */
		char n[8]; // 8
	};

	/**
	 * Entry in the table.
	 */
	struct Entry
	{
		/**
		 * Hash and name.
		 */
		H8 x;

		/**
		 * Payload
		 */
		void* value;
	};

	enum
	{
		/**
	     * Number of entries the capacity is incremented when
	     * there is not enough space in the table.
	     */
		CapacityIncrementer = 4096 / sizeof(Entry),
	};

	Entry* tbl;      ///< Table of entries.
	int    capacity; ///< Current capacity of table.
	int    count;    ///< Number of used entries in table.

	/**
	 * Finds a string in the table.
	 * @param what What to find.
	 * @param ha Internal hash.
	 * @return Index of string if found.
	 * @return -(insertion_point + 1) on if not found.
	 * @warning The string will be truncated to a maximum of 7 bytes.
	 * @warning The search will be case insensitive.
	 */
	int FindIdxInternal(const char* what, H8& ha) const;

public:
	/**
	 * Constructor
	 */
	S8Table() : tbl(0), capacity(0), count(0)
	{
	}

	/**
	 * Destructor.
	 */
	virtual ~S8Table()
	{
		Clobber();
	}

	/**
	 * Sets count to zero.
	 */
	virtual void Clear()
	{
		count = 0;
	}

	/**
	 * Sets count to zero and erases everything
	 */
	virtual void Clobber();

	/**
	 * Get number of entries in use.
	 * @return Number of entries in use.
	 */
	int GetCount() const
	{
		return count;
	}

	/**
	 * Get Capacity of table.
	 * @return Capacity of table.
	 */
	int GetCapacity() const
	{
		return capacity;
	}

	/**
	 * Reserve space.
	 * @param newCapacity Requested capacity.
	 * @return Capacity on success. -1 on failure.
	 */
	int Reserve(int newCapacity);

	/**
	 * Get Value given an index.
	 * @param idx Index
	 * @return value.
	 */
	void* operator[](int idx)
	{
		return tbl[idx].value;
	}

	/**
	* Get Value given an index.
	* @param idx Index
	* @return value.
	*/
	const void* operator[](int idx) const
	{
		return tbl[idx].value;
	}

	/**
	* Get Value given a key
	* @param key key.
	* @return value.
	*/
	void* operator[](const char* key)
	{
		return Get(key);
	}

	/**
	* Get Value given a key
	* @param key key.
	* @return value.
	*/
	const void* operator[](const char* key) const
	{
		return Get(key);
	}

	/**
	 * Gets item by index.
	 * @param idx Index of item.
	 * @return Value of item on success.
	 * @return 0 on failure.
	 */
	void* GetByIdx(int idx)
	{
		if (!tbl || 0 > idx || idx >= count) {
			return 0;
		}
		return tbl[idx].value;
	}

	/**
	 * Gets item by index.
	 * @param idx Index of item.
	 * @return Value of item on success.
	 * @return 0 on failure.
	 */
	const void* GetByIdx(int idx) const
	{
		if (!tbl || 0 > idx || idx >= count) {
			return 0;
		}
		return tbl[idx].value;
	}

	/**
	 * Gets item name by index.
	 * @param idx Index of item.
	 * @param where 8 bytes buffer that will hold the name.
	 * @return where on success.
	 * @return 0 on failure.
	 */
	const char* GetNameByIdx(int idx, char* where) const
	{
		if (!where) {
			return 0;
		}
		*where = 0;
		if (!tbl || 0 > idx || idx >= count) {
			return 0;
		}
		memcpy(where, tbl[idx].x.n, 8);
		return where;
	}

	/**
	 * Finds a string in the table.
	 * @param what What to find.
	 * @return Index of string if found.
	 * @return -(insertion_point + 1) on if not found.
	 * @warning The string will be truncated to a maximum of 7 bytes.
	 * @warning The search will be case insensitive.
	 */
	int FindIdx(const char* what) const
	{
		H8 x;
		return FindIdxInternal(what, x);
	}

	/**
	 * Gets an item.
	 * @param what Key
	 * @param defValue Default value.
	 * @return value of item on succes.
	 * @return defValue on failure.
	 */
	void* Get(const char* what, void* defValue = 0)
	{
		const int idx = FindIdx(what);
		return (0 <= idx) ? tbl[idx].value : defValue;
	}

	/**
	 * Gets an item.
	 * @param what Key
	 * @param defValue Default value.
	 * @return value of item on succes.
	 * @return defValue on failure.
	 */
	const void* Get(const char* what, const void* defValue = 0) const
	{
		const int idx = FindIdx(what);
		return (0 <= idx) ? tbl[idx].value : defValue;
	}

	/**
	 * Gets or add an item.
	 * @param what Key.
	 * @param value Value in case it was not found.
	 * @return value if newly added.
	 * @return Table value if found.
	 * @return 0 on error.
	 */
	void* GetOrAdd(const char* what, void* value);

	/**
	 * Adds an item.
	 * @param what Key
	 * @param value Value to add.
	 * @return Index where the value is on success.
	 * @return -1 on failure.
	 */
	int Add(const char* what, void* value);

	/**
	 * Sets an item
	 * @param what Key.
	 * @param value Value to set to.
	 * @return 0 on out of memory.
	 * @return value if newly added.
	 * @return previous value on replace.
	 */
	void* Set(const char* what, void* value);
};
