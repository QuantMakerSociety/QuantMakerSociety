#pragma once
#include "qms_cs.h"
#include "qms_item.h"

/**
* Tree of items.
* Useful when one needs a hierarchy of items.
*/
struct qms_itemTree
{
	/**
	* Local threads mutex.
	*/
	mutable qms_cs mtx;

	/**
	* Points to first child.
	*/
	qms_item* firstChild;
	// 0 + 8 = 8

	/**
	* Points to last child.
	*/
	qms_item* lastChild;
	// 8 + 8 = 16

	/**
	 * Set to true if ifs sorted.
	 */
	bool isSorted;
	// 16 + 4 = 20

	/**
	* Constructor.
	*/
	qms_itemTree() : firstChild(0), lastChild(0), isSorted(false)
	{
	}

	/**
	* Destructor.
	*/
	~qms_itemTree()
	{
		DeleteAllChildren();
	}

	/**
	* Deletes all children.
	*/
	void DeleteAllChildren();

	/**
	* Locks the mutex, then calls FindItemNL
	* @param name Name of item
	* @return Pointer to item if found. 0 if not found.
	*/
	qms_item* FindItem(const char* name) const;

	/**
	* Locks the mutex, then calls FindItemInsertPointNL
	* @param name Name of item
	* @param found Will be set to true if it was found. false if its the next
	* pointer.
	* @return Pointer to item or next item. 0 if its last item or there was an
	* error.
	*/
	qms_item* FindItemInsertPoint(const char* name, bool& found) const;

	/**
	* Locks the mutex then calls AddItemNL
	* @param name Name of item.
	* @param type Type of item.
	* @return Pointer to item if added or found.
	*/
	qms_item* AddItem(const char* name, int type);

	/**
	 * Adds a node.
	 * @param name Name of node.
	 * @return Pointer to node on success. 0 on failure.
	 */
	qms_item* AddNode(const char* name);

	/**
	 * Finds an item by its name.
	 * @param name Name of item.
	 * @return Pointer to item if found. 0 if not found.
	 */
	qms_item* FindItemNL(const char* name) const;

	/**
	* Finds an item or the pointer to the next item. (For sorted)
	* @param name Name of item
	* @param found Will be set to true if it was found. false if its the next
	* pointer.
	* @return Pointer to item or next item. 0 if its last item or there was an
	* error.
	*/
	qms_item* FindItemInsertPointNL(const char* name, bool& found) const;

	/**
	* Adds an item.
	* @param name Name of item.
	* @param type Type of item.
	* @return Pointer to item if added or found.
	*/
	qms_item* AddItemNL(const char* name, int type);

	/**
	* Locks the mutex then add an item to a node.
	* @param node Parent node of item.
	* @param name Name of item.
	* @param type Type of item.
	* @return Pointer to item if added or found.
	*/
	qms_item* AddItemToNode(qms_item& node, const char* name, int type);

	/**
	 * Locks the mutex then calls RemoveItemNL
	 * @param itm Item to remove.
	 */
	void RemoveItem(qms_item& itm);

	/**
	 * Removes an item without first locing
	 * @param itm
	 */
	void RemoveItemNL(qms_item& itm);

	/**
	 * Dumps the tree to stdout.
	 */
	void Dump() const;

	/**
	 * Makes a copy of the tree.
	 * @return Pointer to treee on success.
	 */
	qms_itemTree* MakeCopy() const;

private:
	qms_itemTree(const qms_itemTree& x);
	qms_itemTree& operator=(const qms_itemTree& x);
};
