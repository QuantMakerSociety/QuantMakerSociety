#pragma once
/**
* @file
*/
#include "qms_mm.h"

#define LOPART(x) (DWORD)(x)

#define HIPART(x) (DWORD)((x) >> 32)

namespace qms {
namespace mm {

int UnMap(LPBYTE& data)
{
	int retval = -1;

	__try {
		LPVOID p = data;
		if (p) {
			data = 0;
			if (!::UnmapViewOfFile(p)) {
				retval = ::GetLastError();
			}
		}
		retval = 0;
	} __finally {
	}

	return retval;
}

void* MapAll(HANDLE hf, int& err, size_t offset, size_t len)
{
	void* p = 0;
	__try {
		p = MapViewOfFile(
		    hf, FILE_MAP_ALL_ACCESS, HIPART(offset), LOPART(offset), len);
		if (p) {
			err = 0;
		} else {
			err = ::GetLastError();
		}
	} __finally {
	}

	return p;
}

void* MapRW(HANDLE hf, int& err, size_t offset, size_t len)
{
	void* p = 0;
	__try {
		p = MapViewOfFile(
		    hf,
		    FILE_MAP_READ | FILE_MAP_WRITE,
		    HIPART(offset),
		    LOPART(offset),
		    len);
		if (p) {
			err = 0;
		} else {
			err = ::GetLastError();
		}
	} __finally {
	}

	return p;
}

void* MapRO(HANDLE hf, int& err, size_t offset, size_t len)
{
	void* p = 0;
	__try {
		p = MapViewOfFile(
		    hf, FILE_MAP_READ, HIPART(offset), LOPART(offset), len);
		if (p) {
			err = 0;
		} else {
			err = ::GetLastError();
		}
	} __finally {
	}

	return p;
}

int Close(HANDLE& hf)
{
	int r = -1;
	__try {
		if (hf) {
			HANDLE h = hf;
			hf       = 0;
			r        = CloseHandle(h);
		} else {
			r = 0;
		}
	} __finally {
	}

	return r;
}

void CloseAndUnMap(HANDLE& hf, LPBYTE& data)
{
	UnMap(data);
	Close(hf);
}

HANDLE CreateNew(const char* name, size_t len, int& err)
{
	HANDLE hf = 0;
	__try {
		if (name && len) {
			hf = CreateFileMappingA(
			    INVALID_HANDLE_VALUE,
			    NULL,
			    PAGE_READWRITE,
			    HIPART(len),
			    LOPART(len),
			    name);
			if (hf) {
				SetNamedSecurityInfoA(
				    (LPSTR)name,
				    SE_KERNEL_OBJECT,
				    DACL_SECURITY_INFORMATION,
				    0,
				    0,
				    NULL,
				    NULL);
				err = 0;
			} else {
				err = ::GetLastError();
			}
		} else {
			err = -1;
		}
	} __finally {
	}
	return hf;
}

HANDLE CreateFromExisting(const char* name, size_t& len, int& err)
{
	HANDLE hf = 0;
	__try {
		if (name) {
			WIN32_FILE_ATTRIBUTE_DATA fda = {0};
			if (GetFileAttributesExA(name, GetFileExInfoStandard, &fda)) {
				hf = CreateFileMappingA(
				    INVALID_HANDLE_VALUE,
				    NULL,
				    PAGE_READWRITE,
				    fda.nFileSizeHigh,
				    fda.nFileSizeLow,
				    name);
				if (hf) {
					SetNamedSecurityInfoA(
					    (LPSTR)name,
					    SE_KERNEL_OBJECT,
					    DACL_SECURITY_INFORMATION,
					    0,
					    0,
					    NULL,
					    NULL);
					err = 0;
					LARGE_INTEGER li;
					if (GetFileSizeEx(hf, &li)) {
						len = li.QuadPart;
					}
				} else {
					err = ::GetLastError();
					len = 0;
				}
			}
		} else {
			err = -1;
		}

	} __finally {
	}
	return hf;
}

HANDLE Open(const char* name, int& err)
{
	HANDLE hf = 0;
	__try {
		err = -1;
		if (name) {
			hf = OpenFileMappingA(FILE_MAP_READ, TRUE, name);
			if (hf) {
				err = 0;
			} else {
				err = GetLastError();
			}
		}
	} __finally {
	}

	return hf;
}

HANDLE OpenRW(const char* name, int& err)
{
	HANDLE hf = 0;
	__try {
		err = -1;
		if (name) {
			hf = OpenFileMappingA(FILE_MAP_READ | FILE_MAP_WRITE, TRUE, name);
			if (hf) {
				err = 0;
			} else {
				err = GetLastError();
			}
		}
	} __finally {
	}

	return hf;
}

int MMVectorBase::CreateFromFile(const char* name)
{
	if (!name || hf) {
		return -1;
	}

	int    r   = 0;
	size_t len = 0;
	if (0 == (hf = mm::CreateFromExisting(name, len, r))) {
		return r;
	}
	if (len < sizeof(MMHdr)) {
		mm::Close(hf);
		return -1;
	}

	if (0 == (data = (LPBYTE)MapRW(hf, r, 0, 0))) {
		mm::Close(hf);
		return r;
	}
	pHdr = (MMHdr*)data;
	return 0;
}

int MMVectorBase::CreateNew(
    const char* name,
    size_t      siz,
    size_t      initialCapacity)
{
	if (!name || hf || 0 == siz || 0 > initialCapacity) {
		return -1;
	}
	int    r        = 0;
	size_t lenArray = siz * (size_t)initialCapacity;
	size_t len      = lenArray + sizeof(MMHdr);
	if (0 == (hf = mm::CreateNew(name, len, r))) {
		return r;
	}

	if (0 == (data = (LPBYTE)MapRW(hf, r, 0, 0))) {
		mm::Close(hf);
		return r;
	}
	pHdr           = (MMHdr*)data;
	pHdr->count    = 0;
	pHdr->itemSize = siz;
	return 0;
}

int MMVectorBase::Open(const char* name, size_t siz)
{
	if (!name) {
		return -1;
	}

	int err = 0;
	hf      = mm::Open(name, err);
	if (!hf) {
		if (0 == err) {
			err = -1;
		}
		return err;
	} else {
		if (0 == (data = (LPBYTE)MapRO(hf, err, 0, 0))) {
			mm::Close(hf);
			return err;
		}
		pHdr = (MMHdr*)data;
		if (siz != pHdr->itemSize) {
			Close();
			return -1;
		}
	}

	return 0;
}

int MMVectorBase::AddItemPtr(const void* p)
{
	if (!p || !writeable || !pHdr) {
		return -1;
	}

	size_t itemSize = pHdr->itemSize;
	size_t idx      = pHdr->count;
	size_t offset   = sizeof(MMHdr) + pHdr->itemSize * idx;
	memcpy(data + offset, p, itemSize);
	pHdr->count++;
	return (int)idx;
}

MMHeap::MMHeap() : hf(0), data(0), pHdr(0)
{
}

MMHeap::~MMHeap()
{
	Close();
}

void MMHeap::Close()
{
	mm::CloseAndUnMap(hf, data);
}

MMHeap::Block* MMHeap::AllocBlock(uint32_t nUnits)
{
	Header& hdr = *pHdr;
	if (!pHdr || 0 == nUnits) {
		return 0;
	}

	if (hdr.maxOffset <= hdr.offsetNext) {
		return 0;
	}

	uint32_t offset = hdr.offsetNext;
	uint32_t avail  = SizeToUnits(hdr.maxOffset - offset);

	if (avail < nUnits) {
		return 0;
	}

	hdr.offsetNext += (nUnits << 4);

	Block* pBlock = (Block*)(data + offset);

	pBlock->startMagic = (uint32_t)Block::START_MAGIC;
	pBlock->offsetNext = 0;
	pBlock->nUnits     = nUnits;
	pBlock->endMagic   = (uint32_t)Block::END_MAGIC_ALLOCATED;
	return pBlock;
}

void* MMHeap::Malloc(size_t siz)
{
	if (!siz || !data || !pHdr) {
		return 0;
	}

	size_t nu = 1 + ((siz + 15) >> 4);
	if (0xFFFFFFFF < nu) {
		return 0;
	}

	uint32_t nUnits = (uint32_t)nu;

	Block* pp = 0;
	for (Block *pb = ToPointer< Block >(pHdr->firstFree); pb;
	     pp = pb, pb = ToPointer< Block >(pb->offsetNext)) {
		if (pb->nUnits >= nUnits) {
			if (pb->nUnits < (nUnits + 2)) {
				// Remove it.
				if (pp) {
					pp->offsetNext = pb->offsetNext;
				} else {
					pHdr->firstFree = pb->offsetNext;
					if (!pHdr->firstFree) {
						pHdr->lastFree = 0;
					}
				}

				// And return it.
				pb->offsetNext = 0;
				return pp + 1;
			} else {
				pb->nUnits -= nUnits;
				Block* pp1      = pb + nUnits;
				pp1->startMagic = (uint32_t)Block::START_MAGIC;
				pp1->offsetNext = 0;
				pp1->nUnits     = nUnits;
				pp1->endMagic   = (uint32_t)Block::END_MAGIC_ALLOCATED;
				return pp1 + 1;
			}
		}
	}

	// If we are here we have not found a block.
	Block* pb = AllocBlock(nUnits);
	if (pb) {
		pb->startMagic = (uint32_t)Block::START_MAGIC;
		pb->offsetNext = 0;
		pb->nUnits     = nUnits;
		pb->endMagic   = Block::END_MAGIC_ALLOCATED;
		return pb + 1;
	}

	return 0;
}

void MMHeap::Free(void* ptr)
{
	if (!ptr || !data || !pHdr) {
		return;
	}
	Header& hdr    = *pHdr;
	Block*  pBlock = (Block*)ptr;
	--pBlock;
	if (pBlock->startMagic != Block::START_MAGIC) {
		return;
	}

	if (pBlock->endMagic != Block::END_MAGIC_ALLOCATED) {
		return;
	}

	uint32_t offset = PointerToOffset(pBlock);
	if (!offset) {
		return;
	}

	uint32_t oNext = offset + (pBlock->nUnits << 4);

	pBlock->offsetNext = 0;
	pBlock->endMagic   = (uint32_t)Block::END_MAGIC_FREE;

	if (hdr.lastFree) {
		Block* pp = 0;
		for (Block *pb = ToPointer< Block >(pHdr->firstFree); pb;
		     pp = pb, pb = ToPointer< Block >(pb->offsetNext)) {
			uint32_t offsetPB = PointerToOffset(pb);
			if (offsetPB >= offset) {
				if (offset == offsetPB) {
					return;
				}
				// Found insertion point.
				pBlock->offsetNext = offsetPB;
				if (pp) {
					uint32_t o1 = pp->offsetNext + (pp->nUnits << 4);
					if (o1 == offset) {
						pp->nUnits += pb->nUnits;
						return;
					}
					pp->offsetNext = offset;
				} else {
					pHdr->firstFree = offset;
				}

				if (offsetPB == oNext) {
					pBlock->nUnits += pb->nUnits;
					pBlock->offsetNext = pb->offsetNext;
				}
				return;
			}
		}
		// Not found, add at end.

		if (hdr.lastFree) {
			Block*   pLast    = ToPointer< Block >(hdr.lastFree);
			uint32_t offsetPB = hdr.lastFree + (pLast->nUnits << 4);
			if (offsetPB == offset) {
				pLast->nUnits += pBlock->nUnits;
				return;
			}
			pLast->offsetNext = offset;
		}
		hdr.lastFree = offset;
	} else {
		hdr.lastFree = pHdr->firstFree = offset;
	}
}

int MMHeap::Create(const char* name, size_t len)
{
	if (len < MIN_FILE_SIZE) {
		len = MIN_FILE_SIZE;
	}

	len = (((len + 15) >> 4) << 4);
	if (MAX_FILE_SIZE < len) {
		len = MAX_FILE_SIZE;
	}

	int err = 0;
	hf      = mm::CreateNew(name, len, err);
	if (!hf) {
		return (err) ? err : -1;
	}

	data = (LPBYTE)mm::MapRW(hf, err);
	if (!data) {
		mm::Close(hf);
		return (err) ? err : -1;
	}

	pHdr             = (Header*)data;
	pHdr->maxOffset  = (uint32_t)len;
	pHdr->offsetNext = sizeof(Header);
	pHdr->firstFree  = 0;
	pHdr->lastFree   = 0;

	return 0;
}

int MMHeap::Open(const char* name)
{
	if (!name) {
		return -1;
	}

	int err = 0;
	hf      = mm::OpenRW(name, err);
	if (!hf) {
		return (err) ? err : -1;
	}
	data = (LPBYTE)mm::MapRW(hf, err);
	if (!data) {
		mm::Close(hf);
		return (err) ? err : -1;
	}
	pHdr = (Header*)data;

	return 0;
}

} // namespace mm

} // namespace qms
