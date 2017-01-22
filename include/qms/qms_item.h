#pragma once
/**
 * @file
 */
#include "qms_cs.h"
#include "qms_string.h"

/**
 * Configuration Item.
 */
__declspec(align(16)) struct qms_item
{
	/**
	 * Types of items.
	 */
	enum eType
	{
		TP_STR,     ///< String.
		TP_INT,     ///< 32 bit signed integer.
		TP_UINT,    ///< 32 bit unsigned integer.
		TP_ACCT,    ///< Account.
		TP_BOOL,    ///< Boolean
		TP_I64,     ///< 64 bit signed integer.
		TP_U64,     ///< 64 bit unsigned integer.
		TP_TIME,    ///< 64 bit unix time.
		TP_DBL,     ///< Double
		TP_BTN,     ///< Button.
		TP_CHAR,    ///< Single character.
		TP_UCHAR,   ///< Single unsigned character.
		TP_NODE,    ///< Node (DO NOT USE in an array)
		TP_MA,      ///< Moving average 1 - 8
		TP_SECONDS, ///< Seconds per bar.
	};

	enum
	{
		MAX_NAME_SIZE = 39,       ///< Maximum size for name.
		MAX_STR_SIZE  = 512 - 64, ///< Maximum size for a string in this item.
	};

	/**
	 * Points to previous item. (If its on a list or tree)
	 */
	qms_item* prev;
	// 0 + 8 = 8

	/**
	 * Points to next item. (If its on a list or tree)
	 */
	qms_item* next;
	// 8 + 8 = 16

	/**
	 * Points to parent item. (If its on tree)
	 */
	qms_item* parent;
	// 16 + 8 = 24

	/**
	 * Name of item.
	 */
	char name[MAX_NAME_SIZE];
	// 24 + 39 = 63

	/**
	 * Type of item.
	 */
	char type;
	// 63 + 1 = 64

	/**
	 * Union of values.
	 */
	union UValues
	{
		bool     b;                 ///< Boolean
		char     c;                 ///< Signed character.
		uint8_t  u8;                ///< Unsigned character.
		int32_t  i32;               ///< Signed 32 bit integer.
		uint32_t u32;               ///< Unsigned 32 bit integer.
		int64_t  i64;               ///< Signed 64 bit integer.
		uint64_t u64;               ///< 64 bit unsigned integer.
		double   dbl;               ///< Signed double.
		__m128   m128;              ///< 128 bit
		char     str[MAX_STR_SIZE]; ///< String of up to (MAX_STR_SIZE - 1)
		                            /// characters.
		/**
		 * Node. In case on requires a hierarchy.
		 */
		struct node_t
		{
			qms_item* firstChild; ///< Points to first child.
			qms_item* lastChild;  ///< Points to last child
			int32_t   idGroup;    ///< Id of group.
			int32_t   idAccount;  ///< id of account.
			int32_t   idSymbol;   ///< Id of symbol. -1 for any symbol.
			bool      isSorted;   ///< Set to true if is a sorted node.
		} node;
	} val; ///< Values
	// 64 + 452 = 512

	/**
	 * Clears the item.
	 */
	void Clear();

	/**
	 * Sets the name of the item.
	 * @param nam
	 */
	void SetName(const char* nam);

	/**
	 * Sets type to TP_INT and then sets val.i32
	 * @param x Value
	 */
	void SetInt(int x);

	/**
	 * Sets the type to TP_BOOL. And then set val.b
	 * @param x Value
	 */
	void SetBool(bool x);

	/**
	 * Set the type to TP_ACCT. And then set val.i32
	 * @param x Value
	 */
	void SetAcct(int x);

	void SetMA(int x);

	/**
	 * Sets the type to TP_BTN. And then sets val.i32
	 * @param x Value
	 */
	void SetBtn(int x);

	/**
	 * Sets the type to TP_INT64. And then sets val.i64
	 * @param x Value
	 */
	void SetInt64(int64_t x);

	/**
	 * Sets the type of TP_DBL. And then sets val.dbl
	 * @param x Value.
	 */
	void SetDbl(double x);

	/**
	 * Sets the type to TP_STR. And then sets val.str
	 * @param str
	 */
	void SetStr(const char* str);

	/**
	 * Default constructor. Set to empty TP_STR
	 */
	qms_item();

	/**
	 * String constructor.
	 * @param nam Name of item.
	 * @param str String for item.
	 */
	qms_item(const char* nam, const char* str);

	/**
	 * Integer constructor.
	 * @param nam Name of item.
	 * @param x Value.
	 */
	qms_item(const char* nam, int x);

	/**
	 * Boolean constructor.
	 * @param nam Name of item.
	 * @param x Value.
	 */
	qms_item(const char* nam, bool x);

	/**
	 * Constructor for button and account.
	 * @param nam Name of item.
	 * @param x Value.
	 * @param t Type. Either TP_ACCT or TP_BTN
	 */
	qms_item(const char* nam, int x, int t);

	/**
	 * Constructor based 64 integer
	 * @param nam Name of item.
	 * @param x Value.
	 */
	qms_item(const char* nam, int64_t x);

	/**
	 * Constructor based on a double,
	 * @param nam Name of item.
	 * @param x Value
	 */
	qms_item(const char* nam, double x);

	/**
	 * Destructor.
	 */
	~qms_item();

	/**
	 * Copy operator.
	 * @param x What to copy.
	 * @return *this
	 */
	qms_item& operator=(const qms_item& x);

	/**
	 * Sets the item type to TP_INT and then sets val.i32
	 * @param x Value
	 * @return *this
	 */
	qms_item& operator=(const int x);

	/**
	 * Sets the item type to TP_INT64 and then sets val.i64
	 * @param x
	 * @return *this
	 */
	qms_item& operator=(const int64_t x);

	/**
	 * Sets the item type to TP_BOOL and then sets val.b
	 * @param x Value
	 * @return *this
	 */
	qms_item& operator=(const bool x);

	/**
	 * Compares the names of two items.
	 * @param x Item to compare to.
	 * @return 0 if equal. 1 if > x. -1 if < x
	 */
	int Compare(const qms_item& x) const;

	/**
	 * Compares item name name with another name (case insensitive)
	 * @param nam Name to compare with.
	 * @return 0 if equal. 1 if > nam. -1 if < nam
	 */
	int Compare(const char* nam) const;

	/**
	 * Check if two items are equal in name.
	 * @param x Second item
	 * @return true if names are equal.
	 */
	bool operator==(const qms_item& x) const;

	/**
	 * > Operator (operates on name only)
	 * @param x Second item
	 * @return true > x. false otherwise.
	 */
	bool operator>(const qms_item& x) const;

	/**
	 * < Operator (operates on name only)
	 * @param x Second item
	 * @return true < x. false otherwise.
	 */
	bool operator<(const qms_item& x) const;

	/**
	 * >= Operator (operates on name only)
	 * @param x Second item
	 * @return true >= x. false otherwise.
	 */
	bool operator>=(const qms_item& x) const;

	/**
	 * <= Operator (operates on name only)
	 * @param x Second item
	 * @return true >= x. false otherwise.
	 */
	bool operator<=(const qms_item& x) const;

	/**
	 * Locks the mutex, then calls FindItemNL
	 * @param name Name of item
	 * @return Pointer to item if found. 0 if not found.
	 */
	qms_item* FindItem(const char* name) const;

	/**
	 * Locks the mutex, then calls FindItemInsertPointNL
	 * @param name Name of item
	 *  @param found Will be set to true if it was found. false if its the next
	 * pointer.
	 * @return Pointer to item or next item. 0 if its last item or there was an
	 * error.
	 */
	qms_item* FindItemInsertPoint(const char* name, bool& found) const;

	/**
	 * Locks the mutex then calls AddItemNL
	 * @param name Name of item.
	 * @param t Type of item.
	 * @return Pointer to item if added or found.
	 */
	qms_item* AddItem(const char* name, int t);

	/**
	 * Adds a node.
	 * @param name Name of node.
	 * @return Pointer to node.
	 */
	qms_item* AddNode(const char* name)
	{
		return AddItem(name, TP_NODE);
	}

	qms_item* AddCopy(const qms_item& itm);

	/**
	 * Adds a child (if this item is a node)
	 * @tparam T Type.
	 * @param name Name
	 * @param x Value
	 * @return Pointer to added item.
	 */
	template < typename T > qms_item* AddChild(const char* name, const T x);

	/**
	 * Adds an integer (if this item is a node)
	 * @param name Name of item.
	 * @param x Value
	 * @return Pointer to added item.
	 */
	template <> qms_item* AddChild< int >(const char* name, const int x)
	{
		qms_item* pChild = AddItem(name, TP_INT);
		if (pChild) {
			pChild->val.i32 = x;
		}
		return pChild;
	}

	/**
	 * Adds an unsigned integer.
	 * @param name Name of item.
	 * @param x Value
	 * @return Pointer to added item.
	 */
	template <>
	qms_item* AddChild< unsigned >(const char* name, const unsigned x)
	{
		qms_item* pChild = AddItem(name, TP_UINT);
		if (pChild) {
			pChild->val.u32 = x;
		}
		return pChild;
	}

	/**
	 * Adds a 64 bit integer.
	 * @param name Name of item.
	 * @param x Value
	 * @return Pointer to added item.
	 */
	template <> qms_item* AddChild< int64_t >(const char* name, const int64_t x)
	{
		qms_item* pChild = AddItem(name, TP_I64);
		if (pChild) {
			pChild->val.i64 = x;
		}
		return pChild;
	}

	/**
	 * Adds 64 bit unsigned integer.
	 * @param name Name of item.
	 * @param x Value
	 * @return Pointer to added item.
	 */
	template <>
	qms_item* AddChild< uint64_t >(const char* name, const uint64_t x)
	{
		qms_item* pChild = AddItem(name, TP_U64);
		if (pChild) {
			pChild->val.u64 = x;
		}
		return pChild;
	}

	/**
	 * Adds a double.
	 * @param name Name of item.
	 * @param x Value
	 * @return Pointer to added item.
	 */
	template <> qms_item* AddChild< double >(const char* name, const double x)
	{
		qms_item* pChild = AddItem(name, TP_DBL);
		if (pChild) {
			pChild->val.dbl = x;
		}
		return pChild;
	}

	/**
	 * Adds a string.
	 * @param name Name of item.
	 * @param x Value
	 * @return Pointer to added item.
	 */
	qms_item* AddString(const char* name, const char* x)
	{
		qms_item* pChild = AddItem(name, TP_STR);
		if (pChild) {
			(void)strcpy_s(pChild->val.str, x);
		}
		return pChild;
	}

	/**
	* If the item is a node. Delete all children.
	*/
	void DeleteChildren();

	/**
	* If the item is a node. Remove all children.
	* @return First child if any. 0 if none.
	*/
	qms_item* RemoveChildren();

	/**
	 * Removes a child (if the item is a node.
	 * @param itm Item to remove.
	 */
	void RemoveChild(qms_item& itm);

	/**
	 * Debugging function that dumps an item to stdout.
	 * @param lvl Level of intentation.
	 */
	void Dump(int lvl) const;

	/**
	 * Converts the type to a string.
	 * @return String describing the type.
	 */
	const char* TypeToString() const;

	/**
	 * Converts an iten to a string,
	 * @param sz Will hold the string.
	 * @param siz Size of sz
	 * @return Len of string on success. -1 on failure.
	 */
	int ItemToString(char* sz, size_t siz) const;

	/**
	 * Check if item is a node.
	 * @return true if its a node. false if its not.
	 */
	bool IsNode() const
	{
		return TP_NODE == type;
	}

	/**
	 * Calls new to create a copy of the item.
	 * @return Pointer to copy on success. 0 on failure.
	 */
	qms_item* MakeCopy() const;

}; // 512 bytes

/**
* Array of items.
*/
typedef qms_tarray< qms_item > qms_itemArray;
