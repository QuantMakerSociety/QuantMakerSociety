#pragma once
/**
 * @file
 */
/**
* Array
* @tparam T Type of item.
*
* Can be thought as a vector of pointer to items.
* @warning It DOES NOT OWN its items. This is done by internal design.
* @warning it does not have a destructor. Again done by internal design.
*/
template < typename T > struct qms_tarray
{
	/**
	* Number of items in the array.
	*/
	uint32_t count; // 0

	/**
	* Capacity of the array
	*/
	uint32_t capacity; // 0 + 4 = 4

	/**
	* Array
	*/
	T** data; // 4 + 4 = 8

	/**
	* Constructor.
	*/
	qms_tarray() : count(0), capacity(0), data(0)
	{
	}

	/**
	* Set count to zero.
	*/
	void Clear()
	{
		count = 0;
	}

	/**
	* Deletes all the item in the array
	* And sets the count to zero.
	*/
	void ClearAndDelete()
	{
		if (!data) {
			capacity = count = 0;
			return;
		}

		while (count) {
			--count;
			delete data[count];
		}
	}

	/**
	* Deletes the pointer array and sets capacity and count to zero.
	*/
	void Clobber()
	{
		if (data) {
			delete[] data;
			data = 0;
		}
		count = capacity = 0;
	}

	/**
	* Deletes all the items in the array,
	* then the pointer array, finally set capacity and count to zero.
	*/
	void ClobberAndDelete()
	{
		if (!data) {
			capacity = count = 0;
			return;
		}

		while (count) {
			--count;
			delete data[count];
		}

		capacity = count = 0;
		delete[] data;
	}

	/**
	* Grows capacity of array
	* @param	newCapacity New capacity.
	*/
	void Reserve(uint32_t newCapacity)
	{
		if (capacity >= newCapacity) {
			return;
		}
		T** pp = new T*[newCapacity];
		if (data) {
			for (uint32_t i = 0; i < count; i++) {
				pp[i] = data[i];
			}
			delete[] data;
		}
		data     = pp;
		capacity = newCapacity;
	}

	/**
	* Adds a reference
	* @param	x Reference
	*/
	void AddReference(T& x)
	{
		AddPtr(&x);
	}

	/**
	* Adds a copy
	* @param	x Source
	* @return Copy
	*/
	T& AddCopy(const T& x)
	{
		T* p = new T(x);
		AddPtr(p);
		return *p;
	}

	/**
	* Adds a new item at the end.
	* @return Reference to new item.
	*/
	T& AddNew()
	{
		T* p = new T();
		AddPtr(p);
		return *p;
	}

	/**
	* Adds a pointer to the end of the array.
	* @param x Pointer to item. Should not be NULL.
	*/
	void AddPtr(T* x)
	{
		if (!x) {
			return;
		}
		if (capacity == count) {
			Reserve(capacity + 128); // 1k extra
		}
		data[count] = x;
		++count;
	}

	/**
	* Finds a pointer in the array.
	* @param x Pointer to find.
	* @return Index of pointer.
	*/
	int FindPtr(const T* x) const
	{
		for (uint32_t i = 0; i < count; i++) {
			if (data[i] == x) {
				return (int)i;
			}
		}
		return -1;
	}

	/**
	* Finds an item equal.
	* @param x Item to find.
	* @return Index of item.
	*/
	int FindEqual(const T& x) const
	{
		for (uint32_t i = 0; i < count; i++) {
			if (*data[i] == x) {
				return (int)i;
			}
		}
		return -1;
	}

	/**
	* Adds a pointer if not found.
	* @param x Pointer to add. Not NULL;
	*/
	void AddPtrIfNotFound(T* x)
	{
		if (!x) {
			return;
		}
		int idx = FindPtr(x);
		if (0 > idx) {
			AddPtr(x);
		}
	}

	/**
	* Removes a pointer.
	* @param x Pointer to remove.
	*/
	void RemovePtr(const T* x)
	{
		if (!data || !count) {
			return;
		}

		for (uint32_t i = 0; i < count; i++) {
			if (data[i] == x) {
				--count;
				for (uint32_t j = i; j < count; j++) {
					data[j] = data[j + 1];
				}
			}
		}
	}

	/**
	* Removes a reference
	* @param	x Reference to remove.
	*/
	void RemoveReference(const T& x)
	{
		RemovePtr(&x);
	}

	/**
	* Given an index, get a pointer.
	* @param idx Index.
	* @return Pointer on success.
	* @return 0 on failure.
	*/
	T* GetPtr(int idx)
	{
		return (0 <= idx && idx < count) ? data[idx] : 0;
	}

	/**
	* Given an index, get a constant pointer.
	* @param idx Index.
	* @return Constant pointer on success.
	* @return 0 on failure.
	*/
	const T* GetConstPtr(int idx) const
	{
		return (0 <= idx && idx < (int)count) ? data[idx] : 0;
	}

	/**
	* Given an index, get an item.
	* @param idx Index. 0 <= idx < count
	* @return Item.
	*/
	T& operator[](int idx)
	{
		return *data[idx];
	}

	/**
	* Given an index, get an item.
	* @param idx Index. 0 <= idx < count
	* @return Item.
	*/
	const T& operator[](int idx) const
	{
		return *data[idx];
	}

	/**
	* Check if empty.
	*/
	bool IsEmpty() const
	{
		return 0 == count;
	}
}; // 8 + 8 = 16
