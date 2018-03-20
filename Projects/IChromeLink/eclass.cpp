#include "stdafx.h"
#include "eclass.h"

namespace eclass{
	DWORD m_pAccessorVfptr;
}

__declspec(naked) void eclass::Copy(void)
{
	__asm{
		mov esi, [esp];
		call TAlloc;
		mov edi, eax;
		pop ecx;
		xchg ecx, esi;
		push ecx;
		push esi;
		push edi;
		call memcpy;
		push edi;
		lea eax, [esp];
		push esi;
		push eax;
		call[ebx + 8];
		pop eax;
		retn;
	}
}

LPVOID __stdcall eclass::TAlloc(int nSize)
{
	return HeapAlloc(GetProcessHeap(), 0, nSize);
}

void __stdcall eclass::memcpy(void *dest, void *src, int size)
{
	::memcpy(dest, src, size);
}

LPVOID eclass::SetFunctionHookAddr(LPVOID lpSrcAddr, LPVOID lpHookAddr)
{
	DWORD dwOldProtect;
	char* lSrcAddr = (char*)lpSrcAddr;
	ULONG nOffset = (*(ULONG*)&lSrcAddr[1]);
	ULONG pAddr = (DWORD)lpSrcAddr + nOffset;
	pAddr += 5;
	LPVOID lpSrcFunction = (LPVOID)pAddr;
	DWORD dwOffset = (DWORD)lpSrcAddr - (DWORD)lpHookAddr;
	dwOffset = ~dwOffset;
	dwOffset -= 4;
	if (VirtualProtectEx((HANDLE)-1, lSrcAddr, 5, PAGE_EXECUTE_READWRITE, &dwOldProtect))
	{
		lSrcAddr[0] = 0xE9;
		*(DWORD*)&lSrcAddr[1] = dwOffset;
		VirtualProtectEx((HANDLE)-1, lSrcAddr, 5, dwOldProtect, NULL);
	}
	return lpSrcFunction;
}

void eclass::SetAccessorClass(DWORD **pNewClass)
{
	DWORD dwVfptr = **pNewClass;
	m_pAccessorVfptr = dwVfptr;
	dwVfptr += sizeof(DWORD);
	DWORD dwSrcAddr = *(DWORD*)dwVfptr;
	dwSrcAddr += 6;
	BYTE *pCoder = (BYTE*)dwSrcAddr;
	if (*pCoder != 0xE9)
		eclass::SetFunctionHookAddr((LPVOID)dwSrcAddr, eclass::Copy);
}