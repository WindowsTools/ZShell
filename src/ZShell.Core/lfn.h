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

BOOL IsFATName(LPTSTR lpFileName); //check file name

DWORD GetNameType(LPTSTR lpName);
BOOL IsLFN(LPTSTR lpName);

BOOL WFFindFirst(LPLFNDTA lpFind, LPTSTR lpName, DWORD dwAttrFilter);
BOOL WFFindNext(LPLFNDTA lpFind);
BOOL WFFFindClose(LPLFNDTA lpFind);

BOOL WFIsDir(LPTSTR lpName);
BOOL LFNMergePath(LPTSTR, LPTSTR);
BOOL IsLFNSelected();

