#pragma once

/*
 * Long File Name ?
*/

typedef struct {
	HANDLE hFindFile;
	DWORD dwAttrFilter;
	DWORD err;
	WIN32_FIND_DATA fd;
	INT nSpaceLeft;
} LFNDTA, * LPLFNDTA, * PLFNDTA;

DWORD GetNameType(LPWSTR lpName);
BOOL IsLFN(LPWSTR lpName);

BOOL WFFindFirst(LPLFNDTA lpFind, LPWSTR lpName, DWORD dwAttrFilter);
BOOL WFFindNext(LPLFNDTA lpFind);
BOOL WFFFindClose(LPLFNDTA lpFind);

BOOL WFIsDir(LPWSTR lpName);
