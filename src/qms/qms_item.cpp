#include "stdafx.h"
#include <qms/qms_assert.h>
#include <qms/qms_item.h>

bool qms_item::operator<=(const qms_item& x) const
{
	return 0 >= Compare(x);
}

bool qms_item::operator>=(const qms_item& x) const
{
	return 0 <= Compare(x);
}

bool qms_item::operator<(const qms_item& x) const
{
	return 0 > Compare(x);
}

bool qms_item::operator>(const qms_item& x) const
{
	return 0 < Compare(x);
}

bool qms_item::operator==(const qms_item& x) const
{
	return 0 == Compare(x);
}

qms_item& qms_item::operator=(const bool x)
{
	SetBool(x);
	return *this;
}

qms_item& qms_item::operator=(const int64_t x)
{
	SetInt64(x);
	return *this;
}

qms_item& qms_item::operator=(const int x)
{
	SetInt(x);
	return *this;
}

qms_item& qms_item::operator=(const qms_item& x)
{
	if (this != &x) {
		SetName(x.name);

		if (IsNode()) {
			DeleteChildren();
		}

		type = x.type;
		memcpy(val.str, x.val.str, sizeof(val.str));

		if (x.IsNode()) {
		}
	}
	return *this;
}

void qms_item::Clear()
{
	if (IsNode()) {
		DeleteChildren();
		return;
	}

	memset(val.str, 0, sizeof(val.str));
}

void qms_item::SetName(const char* nam)
{
	if (nam) {
		strcpy_s(name, sizeof(name), nam);
	} else {
		name[0] = 0;
	}
}

void qms_item::SetInt(int x)
{
	type    = TP_INT;
	val.i32 = x;
}

void qms_item::SetBool(bool x)
{
	type    = TP_BOOL;
	val.i32 = x;
}

void qms_item::SetAcct(int x)
{
	type    = TP_ACCT;
	val.i32 = x;
}

void qms_item::SetMA(int x)
{
	if (0 > x || x > 8) {
		x = 0;
	}
	type    = TP_MA;
	val.i32 = x;
}

void qms_item::SetBtn(int x)
{
	type    = TP_BTN;
	val.i32 = x;
}

void qms_item::SetInt64(int64_t x)
{
	type    = TP_INT;
	val.i64 = x;
}

void qms_item::SetDbl(double x)
{
	type    = TP_DBL;
	val.dbl = x;
}

void qms_item::SetStr(const char* str)
{
	if (str) {
		strcpy_s(val.str, sizeof(val.str) - 1, str);
	} else {
		val.str[0] = 0;
	}
	type = TP_STR;
}

qms_item::qms_item(const char* nam, double x) : qms_item()
{
	SetName(nam);
	SetDbl(x);
}

qms_item::qms_item(const char* nam, int64_t x) : qms_item()
{
	SetName(nam);
	SetInt64(x);
}

qms_item::qms_item(const char* nam, int x, int t) : qms_item()
{
	SetName(nam);
	type    = t;
	val.i32 = x;
}

qms_item::qms_item(const char* nam, bool x) : qms_item()
{
	SetName(nam);
	SetBool(x);
}

qms_item::qms_item(const char* nam, int x) : qms_item()
{
	SetName(nam);
	SetInt(x);
}

qms_item::qms_item(const char* nam, const char* str) : qms_item()
{
	SetName(nam);
	SetStr(str);
}

qms_item::qms_item()
{
	prev    = 0;
	next    = 0;
	parent  = 0;
	name[0] = 0;
	type    = TP_STR;
	val.i64 = 0;
}

qms_item::~qms_item()
{
	if (IsNode()) {
		DeleteChildren();
	}
}

int qms_item::Compare(const char* nam) const
{
	if (!nam) {
		return 1;
	}

	if (name == nam) {
		return 0;
	}
	return _stricmp(name, nam);
}

int qms_item::Compare(const qms_item& x) const
{
	if (this == &x) {
		return 0;
	}
	return _stricmp(name, x.name);
}

qms_item* qms_item::FindItem(const char* name) const
{
	if (!name || TP_NODE != type) {
		return 0;
	}

	const UValues::node_t& node = val.node;

	if (node.isSorted) {
		for (const qms_item* p = node.firstChild; p; p = p->next) {
			int r = p->Compare(name);
			if (0 <= r) {
				if (0 == r) {
					return (qms_item*)p;
				}
				return 0;
			}
		}
	} else {
		for (const qms_item* p = node.firstChild; p; p = p->next) {
			int r = p->Compare(name);
			if (0 == r) {
				return (qms_item*)p;
			}
		}
	}

	return 0;
}

qms_item* qms_item::FindItemInsertPoint(const char* name, bool& found) const
{
	found = false;
	if (!name || TP_NODE != type) {
		return 0;
	}

	const UValues::node_t& node = val.node;

	if (node.isSorted) {
		for (const qms_item* p = val.node.firstChild; p; p = p->next) {
			int r = p->Compare(name);
			if (0 <= r) {
				if (0 == r) {
					found = true;
				}
				return (qms_item*)p;
			}
		}
	} else {
		for (const qms_item* p = val.node.firstChild; p; p = p->next) {
			int r = p->Compare(name);
			if (0 == r) {
				found = true;
				return (qms_item*)p;
			}
		}
	}
	return 0;
}

qms_item* qms_item::AddItem(const char* name, int t)
{
	if (!name || TP_NODE != type) {
		return 0;
	}

	UValues::node_t& node = val.node;

	bool      found = false;
	qms_item* pNext = FindItemInsertPoint(name, found);
	if (pNext && found) {
		// Found it.
		return pNext;
	}

	qms_item* pItem = new (std::nothrow) qms_item();
	if (!pItem) {
		return 0;
	}

	qms_item& itm = *pItem;
	itm.SetName(name);
	itm.Clear();
	itm.type   = t;
	itm.next   = pNext;
	itm.parent = this;

	if (itm.IsNode()) {
		itm.val.node.isSorted = node.isSorted;
	}

	if (pNext) {
		itm.prev    = pNext->prev;
		pNext->prev = pItem;
	} else {
		itm.prev       = node.lastChild;
		node.lastChild = pItem;
	}

	if (itm.prev) {
		itm.prev->next = pItem;
	} else {
		node.firstChild = pItem;
	}

	return pItem;
}

void qms_item::DeleteChildren()
{
	if (TP_NODE != type) {
		return;
	}

	qms_item* p        = val.node.lastChild;
	val.node.lastChild = val.node.firstChild = 0;

	while (p) {
		if (TP_NODE == p->type) {
			p->DeleteChildren();
		}
		qms_item* pp = p->prev;
		delete p;
		p = pp;
	}
}

qms_item* qms_item::RemoveChildren()
{
	if (TP_NODE == type) {
		qms_item* pRet      = val.node.firstChild;
		val.node.firstChild = val.node.lastChild = 0;
		if (pRet) {
			for (qms_item* p = pRet; p; p = p->next) {
				p->parent = 0;
			}
		}
		return pRet;
	}

	return 0;
}

void qms_item::RemoveChild(qms_item& itm)
{
	qms_assert(itm.parent == this);
	qms_assert(TP_NODE == type);
	if (TP_NODE != type) {
		return;
	}

	if (!val.node.firstChild) {
		return;
	}

	qms_assert(val.node.lastChild);

	if (val.node.firstChild == &itm) {
		qms_assert(!itm.prev);

		if (!itm.next) {
			qms_assert(val.node.lastChild == val.node.firstChild);
			val.node.firstChild = val.node.lastChild = 0;
		} else {
			val.node.firstChild       = itm.next;
			val.node.firstChild->prev = 0;
		}

		itm.prev = itm.next = 0;
		itm.parent          = 0;
		return;
	}

	if (val.node.lastChild == &itm) {
		qms_assert(!itm.next);
		qms_assert(itm.prev);
		val.node.lastChild       = itm.prev;
		val.node.lastChild->next = 0;
		itm.prev = itm.next = 0;
		itm.parent          = 0;
		return;
	}

	qms_item* pp = itm.prev;
	qms_item* pn = itm.next;
	itm.prev = itm.next = 0;
	pp->next            = pn;
	pn->prev            = pp;
}

void qms_item::Dump(int lvl) const
{
	static char sz1024[1024];

	for (int i = 0; i < lvl; i++) {
		putchar(' ');
	}

	ItemToString(sz1024, sizeof(sz1024));
	printf(
	    "%p : %s : parent=%p, prev=%p, next=%p, type=%s, val=%s\n",
	    this,
	    name,
	    parent,
	    prev,
	    next,
	    TypeToString(),
	    sz1024);

	if (TP_NODE == type) {
		for (const qms_item* p = val.node.firstChild; p; p = p->next) {
			p->Dump(lvl + 1);
		}
	}
}

const char* qms_item::TypeToString() const
{
	const char* szNames[TP_MA + 1] = {
	    "TP_STR",   // str
	    "TP_INT",   // i32
	    "TP_UINT",  // u32
	    "TP_ACCT",  // i32
	    "TP_BOOL",  // b
	    "TP_I64",   // i64
	    "TP_U64",   // u64
	    "TP_TIME",  // t
	    "TP_DBL",   // dbl
	    "TP_BTN",   // i32
	    "TP_CHAR",  // c
	    "TP_UCHAR", // u8
	    "TP_NODE",
	    "TP_MA",
	};

	if (0 > type || TP_MA < type) {
		return "TP_UNKNOWN";
	}

	return szNames[type];
}

int qms_item::ItemToString(char* sz, size_t siz) const
{
	switch (type) {
	case TP_STR: return strcpy_s(sz, siz, val.str);
	case TP_INT: return sprintf_s(sz, siz, "%d", val.i32);
	case TP_UINT: return sprintf_s(sz, siz, "%u", val.u32);
	case TP_ACCT: return sprintf_s(sz, siz, "ACCT %d", val.i32);
	case TP_MA: return sprintf_s(sz, siz, "MA %d", val.i32);
	case TP_BOOL:
		if (val.b) {
			return sprintf_s(sz, siz, "true");
		}
		return sprintf_s(sz, siz, "false");

	case TP_I64: return sprintf_s(sz, siz, "%I64d", val.i64);
	case TP_U64: return sprintf_s(sz, siz, "%I64u", val.u64);
	case TP_TIME: return sprintf_s(sz, siz, "TIME %I64u", val.u64);
	case TP_DBL: return sprintf_s(sz, siz, "%f", val.dbl);
	case TP_BTN: return sprintf_s(sz, siz, "BTN %d", val.i32);
	case TP_CHAR: return sprintf_s(sz, siz, "%d", (int)val.c);
	case TP_UCHAR: return sprintf_s(sz, siz, "%d", (int)val.u8);
	case TP_NODE:
		return sprintf_s(
		    sz,
		    siz,
		    "head=%p, tail=%p",
		    val.node.firstChild,
		    val.node.lastChild);
	}
	return sprintf_s(sz, siz, "");
}

qms_item* qms_item::MakeCopy() const
{
	qms_item* pItem = new (std::nothrow) qms_item();
	if (pItem) {
		memcpy(pItem, this, sizeof(qms_item));
		pItem->parent = 0;
		pItem->prev   = 0;
		pItem->next   = 0;
		if (IsNode()) {
			UValues::node_t& node = pItem->val.node;
			for (const qms_item* cp = val.node.firstChild; cp; cp = cp->next) {
				qms_item* pChild = cp->MakeCopy();
				if (pChild) {
					pChild->parent = pItem;
					pChild->prev   = node.lastChild;
					pChild->next   = 0;
					if (node.lastChild) {
						node.lastChild->next = pChild;
					} else {
						node.firstChild = pChild;
					}
					node.lastChild = pChild;
				}
			}
		}
	}

	return pItem;
}

qms_item* qms_item::AddCopy(const qms_item& itm)
{
	if (!IsNode()) {
		return 0;
	}

	bool      found = false;
	qms_item* pNext = FindItemInsertPoint(name, found);

	qms_item* pItem = itm.MakeCopy();

	if (!pItem) {
		return 0;
	}

	qms_item& i2 = *pItem;
	i2.parent    = this;

	UValues::node_t& node = val.node;
	if (i2.IsNode()) {
		i2.val.node.isSorted = node.isSorted;
	}

	if (pNext) {
		i2.prev     = pNext->prev;
		pNext->prev = pItem;
	} else {
		i2.prev        = node.lastChild;
		node.lastChild = pItem;
	}

	if (i2.prev) {
		i2.prev->next = pItem;
	} else {
		node.firstChild = pItem;
	}

	return pItem;
}
