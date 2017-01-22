#pragma once
#include "qms_itemTree.h"

void qms_itemTree::DeleteAllChildren()
{
	qms_item* p = lastChild;
	firstChild = lastChild = 0;
	while (p) {
		qms_item* pp = p->prev;
		delete p;
		p = pp;
	}
}

qms_item* qms_itemTree::FindItem(const char* name) const
{
	qms_cs::Lock lock(mtx);
	return FindItemNL(name);
}

qms_item* qms_itemTree::FindItemInsertPoint(const char* name, bool& found) const
{
	qms_cs::Lock lock(mtx);
	return FindItemInsertPointNL(name, found);
}

qms_item* qms_itemTree::AddItem(const char* name, int type)
{
	qms_cs::Lock lock(mtx);
	return AddItemNL(name, type);
}

qms_item* qms_itemTree::AddNode(const char* name)
{
	return AddItem(name, qms_item::TP_NODE);
}

qms_item* qms_itemTree::FindItemNL(const char* name) const
{
	if (isSorted) {
		for (const qms_item* p = firstChild; p; p = p->next) {
			int r = p->Compare(name);
			if (0 <= r) {
				if (0 == r) {
					return (qms_item*)p;
				}
				return 0;
			}
		}
	} else {
		for (const qms_item* p = firstChild; p; p = p->next) {
			int r = p->Compare(name);
			if (0 == r) {
				return (qms_item*)p;
			}
		}
	}
	return 0;
}

qms_item*
    qms_itemTree::FindItemInsertPointNL(const char* name, bool& found) const
{
	found = false;

	if (isSorted) {
		for (const qms_item* p = firstChild; p; p = p->next) {
			int r = p->Compare(name);
			if (0 <= r) {
				if (0 == r) {
					found = true;
				}
				return (qms_item*)p;
			}
		}
	} else {
		for (const qms_item* p = firstChild; p; p = p->next) {
			int r = p->Compare(name);
			if (0 == r) {
				found = true;
				return (qms_item*)p;
			}
		}
	}
	return 0;
}

qms_item* qms_itemTree::AddItemNL(const char* name, int type)
{
	if (!name) {
		return 0;
	}

	bool      found = false;
	qms_item* pNext = FindItemInsertPointNL(name, found);
	if (pNext && found) {
		// Already have it.
		return pNext;
	}

	qms_item* pItem = new (std::nothrow) qms_item();
	if (!pItem) {
		return 0;
	}

	qms_item& itm = *pItem;
	itm.SetName(name);
	itm.Clear();
	itm.type = type;
	itm.next = pNext;

	if (itm.IsNode()) {
		itm.val.node.isSorted = isSorted;
	}

	if (pNext) {
		itm.prev    = pNext->prev;
		pNext->prev = pItem;
	} else {
		itm.prev  = lastChild;
		lastChild = pItem;
	}

	if (itm.prev) {
		itm.prev->next = pItem;
	} else {
		firstChild = pItem;
	}

	return pItem;
}

void qms_itemTree::RemoveItem(qms_item& itm)
{
	qms_cs::Lock lock(mtx);
	RemoveItemNL(itm);
}

void qms_itemTree::RemoveItemNL(qms_item& itm)
{
	if (itm.parent) {
		itm.parent->RemoveChild(itm);
		return;
	}

	if (!firstChild) {
		return;
	}

	if (firstChild == &itm) {
		qms_assert(!itm.prev);
		firstChild = itm.next;
		if (firstChild) {
			firstChild->prev = 0;
		} else {
			qms_assert(lastChild == &itm);
			lastChild = 0;
		}
		return;
	}

	if (lastChild == &itm) {
		qms_assert(!itm.next);
		lastChild = itm.prev;
		qms_assert(lastChild);
		lastChild->next = 0;
		return;
	}

	qms_item* pp = itm.prev;
	qms_item* pn = itm.next;

	qms_assert(pp);
	qms_assert(pn);

	pp->next = pn;
	pn->prev = pp;
	itm.prev = itm.next = 0;
}

void qms_itemTree::Dump() const
{
	qms_cs::Lock lock(mtx);

	for (const qms_item* p = firstChild; p; p = p->next) {
		p->Dump(0);
	}
}

qms_itemTree* qms_itemTree::MakeCopy() const
{
	qms_cs::Lock  lock(mtx);
	qms_itemTree* pCopy = new qms_itemTree();
	if (!pCopy) {
		return 0;
	}

	qms_itemTree& it = *pCopy;
	it.isSorted      = isSorted;

	for (const qms_item* cp = firstChild; cp; cp = cp->next) {
		qms_item* pItem = cp->MakeCopy();
		if (pItem) {
			pItem->parent = 0;
			pItem->prev   = it.lastChild;
			pItem->next   = 0;

			if (it.lastChild) {
				it.lastChild->next = pItem;
			} else {
				it.firstChild = pItem;
			}
			it.lastChild = pItem;
		}
	}

	return pCopy;
}
