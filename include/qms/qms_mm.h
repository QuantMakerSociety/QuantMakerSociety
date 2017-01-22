#pragma once
/**
* @file
*/
#ifndef _WINDOWS_
#error Windows.h must be included before this header.
#endif
#include <stdint.h>

namespace qms {

/**
 * Memory mapped files namespace.
 */
namespace mm {

/**
 * Unmaps a data pointer.
 * @param data Pointer to unmap.
 * @return 0 on success.
 * @return < 0 on internal error.
 * @return > 0 on windows error.
 */
int UnMap(LPBYTE& data);

/**
 * Maps a memory area of a memory mapped file with all access.
 * @param hf Handle of file.
 * @param err Will hold any error.
 * @param offset Offset to map.
 * @param len Length to map.
 * @return Pointer to mapped area on success.
 * @return 0 on failure.
 */
void* MapAll(HANDLE hf, int& err, size_t offset = 0, size_t len = 0);

/**
* Maps a memory area of a memory mapped file with read and write access.
* @param hf Handle of file.
* @param err Will hold any error.
* @param offset Offset to map.
* @param len Length to map.
* @return Pointer to mapped area on success.
* @return 0 on failure.
*/
void* MapRW(HANDLE hf, int& err, size_t offset = 0, size_t len = 0);

/**
* Maps a memory area of a memory mapped file with only read access.
* @param hf Handle of file.
* @param err Will hold any error.
* @param offset Offset to map.
* @param len Length to map.
* @return Pointer to mapped area on success.
* @return 0 on failure.
*/
void* MapRO(HANDLE hf, int& err, size_t offset, size_t len = 0);

/**
 * Calls CloseHandle and sets the handle to zero.
 * @param hf Handle.
 * @return result of CloseHandle
 */
int Close(HANDLE& hf);

/**
 * Unmaps data, setting pointer to zero. Then close the handle and set it to
 * zero.
 * @param hf Handle
 * @param data Data
 */
void CloseAndUnMap(HANDLE& hf, LPBYTE& data);

/**
 * Creates a new map file.
 * @param name Path to map.
 * @param len Length of map.
 * @param err Will hold the error code.
 * @return Handle on success.
 * @return 0 on failure.
 */
HANDLE CreateNew(const char* name, size_t len, int& err);

/**
* Creates a new map file from an existing file.
* @param name Path to map.
* @param err Will hold the error code.
* @return Handle on success.
* @return 0 on failure.
*/
HANDLE CreateFromExisting(const char* name, int& err);

/**
 * Opens an existing map in read only mode.
 * @param name Path to map.
 * @param err Will hold the error code.
 * @return Handle on success.
 * @return 0 on failure.
 */
HANDLE Open(const char* name, int& err);

/**
 * Opens an existing map in read and write mode.
 * @param name Path to map.
 * @param err Will hold the error code.
 * @return Handle on success.
 * @return 0 on failure.
 */
HANDLE OpenRW(const char* name, int& err);

/**
 * Header for a memory mapped vector.
 */
struct MMHdr
{
	/**
	 * Number of items in the vector.
	 */
	size_t count;

	/**
	 * Size of one item.
	 */
	size_t itemSize;
};

/**
 * Base for memory mapped vector.
 */
class MMVectorBase
{
protected:
	/**
	 * Handle of memory mapped file.
	 */
	HANDLE hf;

	/**
	 * Pointer to mapped data.
	 */
	LPBYTE data;

	/**
	 * Pointer to header.
	 */
	MMHdr* pHdr;

	/**
	 * If true, is writable.
	 */
	bool writeable;

public:
	/**
	 * Constructor.
	 */
	MMVectorBase() : hf(0), data(0), pHdr(0), writeable(false)
	{
	}

	/**
	 * Destructor.
	 */
	~MMVectorBase()
	{
		Close();
	}

	/**
	 * Closes the vector.
	 */
	void Close()
	{
		pHdr = 0;
		CloseAndUnMap(hf, data);
	}

	/**
	 * Creates from a previously existing file.
	 * @param name Path to file.
	 * @return 0 on success.
	 * @return -1 if internal error.
	 * @return > 0 if windows error.
	 */
	int CreateFromFile(const char* name);

	/**
	 * Creates a new memory mapped file.
	 * @param name Path to file.
	 * @param siz Size of one item.
	 * @param initialCapacity Initial capacity
	 * @return 0 on success.
	 * @return -1 if internal error.
	 * @return > 0 if windows error.
	 */
	int CreateNew(const char* name, size_t siz, size_t initialCapacity);

	/**
	 * Opens an existing vector in read only mode.
	 * @param name Path to file.
	 * @param siz Size of one item.
	 * @return 0 on success.
	 * @return -1 if internal error.
	 * @return > 0 if windows error.
	 */
	int Open(const char* name, size_t siz);

	/**
	 * Adds an item.
	 * @param p Pointer to item.
	 * @return 0 on success.
	 * @return -1 if internal error.
	 * @return > 0 if windows error.
	 */
	int AddItemPtr(const void* p);

	/**
	 * Get number of items in vector.
	 * @return Number of items.
	 */
	int GetCount() const
	{
		return (pHdr) ? (int)pHdr->count : 0;
	}
};

/**
 * Id to Offset
 */
struct IdToOffet
{
	/**
	 * Id
	 */
	uint64_t id;

	/**
	 * Offset
	 */
	intptr_t offset;
};

/**
 * Memory mapped vector.
 * @tparam T Type of item.
 */
template < typename T > class MMVector : protected MMVectorBase
{
private:
	/**
	 * Pointer to items.
	 */
	T* item;

public:
	/**
	 * Default Constructor.
	 */
	MMVector() : MMVectorBase(), item(0)
	{
	}

	/**
	 * Destructor.
	 */
	~MMVector()
	{
		Close();
	}

	/**
	 * Flushes a view.
	 * @param base Base of view.
	 * @param len length of view.
	 */
	void FlushView(LPVOID base, size_t len)
	{
		if (hf && data) {
			(void)FlushViewOfFile(base, len);
		}
	}

	/**
	 * Closes the map.
	 */
	void Close()
	{
		item = 0;
		MMVectorBase::Close();
	}

	/**
	 * Creates from a file.
	 * @param name path to file.
	 * @return 0 on success.
	 * @return < 0 on Internal error.
	 * @return > 0 on windows error.
	 */
	int CreateFromFile(const char* name)
	{
		int r = MMVectorBase::CreateFromFile(name);
		if (!r) {
			item = (T*)(pHdr + 1);
		}
		writeable = true;
		return 0;
	}

	/**
	* Creates new
	* @param name path to file.
	* @param initialCapacity Initial capacity.
	* @return 0 on success.
	* @return < 0 on Internal error.
	* @return > 0 on windows error.
	*/
	int CreateNew(const char* name, int32_t initialCapacity)
	{
		int r = MMVectorBase::CreateNew(name, sizeof(T), initialCapacity);
		if (!r) {
			item = (T*)(pHdr + 1);
		}
		writeable = true;
		return r;
	}

	/**
	 * Opens in read only mode.
	 * @param name path to file.
	 * @return 0 on success.
	 * @return < 0 on Internal error.
	 * @return > 0 on windows error.
	 */
	int Open(const char* name)
	{
		int r = MMVectorBase::Open(name, sizeof(T));
		if (!r) {
			item = (T*)(pHdr + 1);
		}
		writeable = false;
		return r;
	}

	/**
	 * Adds an item
	 * @param x Item to add.
	 * @return Index on success.
	 * @return -1 on failure.
	 *
	 */
	int AddItem(const T& x)
	{
		if (!item || !writeable || !pHdr) {
			return -1;
		}

		int count   = (int)pHdr->count;
		item[count] = x;
		pHdr->count++;
		return count;
	}

	/**
	 * Adds an item
	 * @return Item.
	 */
	T& AddItem()
	{
		int count = (int)pHdr->count;
		pHdr->count++;
		return item[count];
	}

	/**
	 * Get Number of items.
	 * @return Number of items.
	 */
	int GetCount() const
	{
		return (pHdr) ? (int)pHdr->count : 0;
	}

	/**
	 * Get Capacity
	 * @return Capacity
	 */
	int GetCapacity() const
	{
		return (pHdr) ? pHdr->capacity : 0;
	}

	/**
	 * Access an item
	 * @param idx Index of item
	 * @return Item
	 */
	T& operator[](int idx)
	{
		return item[idx];
	}

	/**
	* Access an item
	* @param idx Index of item
	* @return Item
	*/
	const T& operator[](int idx) const
	{
		return item[idx];
	}

	/**
	 * Check if its open and the data has being mapped.
	 * @return true if open
	 * @return false otherwise
	 */
	bool IsOpen() const
	{
		return 0 != data;
	}

	/**
	 * Check if its writeable
	 * @return true if its writeable
	 * @return false if its not.
	 */
	bool IsWriteble() const
	{
		return writeable;
	}
};

/**
* Heap based on a memory mapped file.
*/
class MMHeap
{
public:
	enum
	{
		/**
	    * Minimum size of file.
	    */
		MIN_FILE_SIZE = 64 * 1024 * 1024,

		/**
	    * Maximum file size.
	    */
		MAX_FILE_SIZE = (1024 * 1024 * 1024),
	};

private:
	/**
	* Header of heap.
	*/
	struct Header
	{
		/**
		* Maximum offset
		* This equals to the size of file.
		*/
		uint32_t maxOffset;

		/**
		* Offset of next available memory.
		*/
		uint32_t offsetNext;

		/**
		* Offset of first block free.
		*/
		uint32_t firstFree;

		/**
		* Offset of last block free.
		*/
		uint32_t lastFree;
	};

	/**
	* Block of memory.
	*/
	struct Block
	{
		enum
		{
			/**
		    * Marks the start of a block.
		    */
			START_MAGIC = 0xCAFEF00D,

			/**
		    * Marks the end of a block
		    * Block is allocated.
		    */
			END_MAGIC_ALLOCATED = 0xBADF00D,

			/**
		    * Marks the end of a block.
		    * Block is free.
		    */
			END_MAGIC_FREE = 0xDEADBEEF,
		};
		/**
		* Marks the start of a block.
		*/
		uint32_t startMagic;

		/**
		* Offset of next block
		*/
		uint32_t offsetNext;

		/**
		* Number of units
		*/
		uint32_t nUnits;

		/**
		* Marks the end of this block header.
		*/
		uint32_t endMagic;
	};

	/**
	* Handle of file.
	*/
	HANDLE hf;

	/**
	* Pointer to mapped data.
	*/
	LPBYTE data;

	/**
	* Pointer to header.
	*/
	Header* pHdr;

public:
	/**
	* Constructor.
	*/
	MMHeap();

	/**
	* Destructor.
	*/
	~MMHeap();

	/**
	* Closes the heap
	*/
	void Close();

	/**
	* Creates the heap.
	* @param name Name of heap.
	* @param len Length of heap.
	* @return 0 on success.
	* @return -1 on internal error.
	* @return >0 Windows error code.
	*/
	int Create(const char* name, size_t len);

	/**
	* Opens heap.
	* @param name Name of heap.
	* @return 0 on success.
	* @return -1 on internal error.
	* @return >0 Windows error code.
	*/
	int Open(const char* name);

	/**
	* Allocates a memory block.
	* @param nUnits Number of units.
	* @return Pointer to block on success.
	* @return 0 on failure.
	*/
	Block* AllocBlock(uint32_t nUnits);

	/**
	* Converts a size to units.
	* @param size Size to convert.
	* @return Number of units.
	*/
	static inline uint32_t SizeToUnits(uint32_t size)
	{
		return (size + 15) >> 4;
	}

	/**
	* Converts number of units to a size.
	* @param nUnits Number of units.
	* @return Size
	*/
	static inline uint32_t UnitsToSize(uint32_t nUnits)
	{
		return nUnits << 4;
	}

	/**
	* Converts a pointer to an offset relative to data.
	* @param ptr Pointer to convert.
	* @return Offet on success.
	* @return 0 on failure.
	*/
	uint32_t PointerToOffset(const void* ptr) const
	{
		if (ptr && data) {
			intptr_t delta = (intptr_t)ptr;
			delta -= (intptr_t)data;
			if (0 >= delta || delta > 0xFFFFFFFF) {
				return 0;
			}

			return (uint32_t)delta;
		}
		return 0;
	}

	/**
	* Converts an offset to a pointer.
	* @param offset Offset
	* @return Pointer.
	*/
	void* OffsetToVoidPointer(uint32_t offset)
	{
		if (0 != offset && 0 != data) {
			return (data + offset);
		}
		return 0;
	}

	/**
	 * Converts an offset to a pointer.
	 * @tparam T Type of pointer.
	 * @param offset Offset in memory mapped file.
	 * @return Pointer of type T.
	 */
	template < typename T > T* ToPointer(uint32_t offset)
	{
		if (0 != offset && 0 != data) {
			return (T*)(data + offset);
		}
		return 0;
	}

	/**
	* Allocates a memory block.
	* @param siz Size of memory block.
	* @return Pointer to memory block on success.
	* @return 0 on failure.
	*/
	void* Malloc(size_t siz);

	/**
	* Frees a memory block.
	* @param ptr Pointer to memory block
	*/
	void Free(void* ptr);

	/**
	* Allocates a block of memory, returning its offset.
	* @param siz Size of memory.
	* @return Offset on success.
	* @return 0 on failure.
	*/
	uint32_t MallocOffset(size_t siz)
	{
		return PointerToOffset(Malloc(siz));
	}

	/**
	* Frees a block of memory using its offset.
	* @param offset Offset
	*/
	void FreeOffset(uint32_t offset)
	{
		return Free(OffsetToVoidPointer(offset));
	}
};

} // namespace mm
} // namespace qms
