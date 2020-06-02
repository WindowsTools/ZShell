#include "pch.h"
#include "lfn.h"
#include "core.h"
#include "wfcopy.h"
#include "wfutil.h"

BOOL IsFATName(LPTSTR lpFileName)
{
	ULONG i, n, name_length, ext_length;
	BOOLEAN dot_yet;
	LPWSTR p;

	n = lstrlen(lpFileName);
	p = (LPWSTR)lpFileName;
	name_length = n;
	ext_length = 0;

	if (n > 12)
		return FALSE;

	dot_yet = FALSE;

	for (i = 0; i < n; i++)
	{
		if (p[i] < 32)
			return FALSE;

		switch (p[i])
		{
		case CHAR_STAR:
		case CHAR_QUESTION:
		case CHAR_SLASH:
		case CHAR_BACKSLASH:
		case CHAR_PIPE:
		case CHAR_COMMA:
		case CHAR_SEMICOLON:
		case CHAR_COLON:
		case CHAR_PLUS:
		case CHAR_EQUAL:
		case CHAR_LESS:
		case CHAR_GREATER:
		case CHAR_OPENBRACK:
		case CHAR_CLOSEBRACK:
		case CHAR_DQUOTE:
			return FALSE;
		case CHAR_DOT:
			if (dot_yet)
				return FALSE;

			dot_yet = TRUE;
			name_length = 1;
			ext_length = n - i - 1;
		default:
			break;
		}
	}

	if (name_length == 0)
	{
		return dot_yet && n == 1;
	}

	if (ext_length == 0)
	{
		return dot_yet == 0;
	}

	if (ext_length > 3 || p[name_length + 1 + ext_length - 1] == CHAR_SPACE)
	{
		return FALSE;
	}

	return TRUE;
}

DWORD GetNameType(LPTSTR lpName)
{
	if (CHAR_COLON == *(lpName + 1))
	{
		if (!IsLFNDrive(lpName))
		{
			return FILE_83_CI;
		}	
	}
	else if (IsFATName(lpName))
	{
		return FILE_83_CI;
	}
	return FILE_LONG;
}

BOOL IsLFN(LPTSTR lpName)
{
	return IsFATName(lpName) == FALSE;
}

/*
 *  Wow64DisableWow64FsRedirection(32-bit application?)
 */
BOOL WFFindFirst(LPLFNDTA lpFind, LPTSTR lpName, DWORD dwAttrFilter)
{
	INT nLen;
	LPTSTR pEnd;

	PVOID oldValue;
	Wow64DisableWow64FsRedirection(&oldValue);

	if (dwAttrFilter & ~(FILE_ATTRIBUTE_DIRECTORY | FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM) == 0)
	{
		lpFind->hFindFile = FindFirstFileEx(lpName, FindExInfoStandard, &lpFind->fd, FindExSearchLimitToDirectories, NULL, 0);
	}
	else
	{
		lpFind->hFindFile = FindFirstFile(lpName, &lpFind->fd);
	}

	if (lpFind->hFindFile == INVALID_HANDLE_VALUE)
	{
		lpFind->err = GetLastError();
	}
	else
	{
		lpFind->err = 0;
	}
		
	dwAttrFilter |= FILE_ATTRIBUTE_ARCHIVE | FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_NORMAL |
		FILE_ATTRIBUTE_REPARSE_POINT | FILE_ATTRIBUTE_TEMPORARY |
		FILE_ATTRIBUTE_COMPRESSED | FILE_ATTRIBUTE_ENCRYPTED | FILE_ATTRIBUTE_NOT_CONTENT_INDEXED;

	Wow64RevertWow64FsRedirection(oldValue);

	nLen = lstrlen(lpName);
	pEnd = &lpName[nLen - 1];

	while (CHAR_BACKSLASH != *pEnd)
	{
		pEnd--;
		nLen--;
	}

	lpFind->nSpaceLeft = MAX_PATH - nLen - 1;

	if (lpFind->hFindFile != INVALID_HANDLE_VALUE)
	{
		lpFind->dwAttrFilter = dwAttrFilter;

		if ((~dwAttrFilter & lpFind->fd.dwFileAttributes == 0L) || WFFindNext(lpFind))
			return TRUE;
		else
		{
			WFFindClose(lpFind);
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}
}

BOOL WFFindNext(LPLFNDTA lpFind)
{
	PVOID oldValue;
	Wow64DisableWow64FsRedirection(&oldValue);

	while (FindNextFile(lpFind->hFindFile, &lpFind->fd)) 
	{

		lpFind->fd.dwFileAttributes &= ATTR_USED;


		if ((lpFind->fd.dwFileAttributes & ~lpFind->dwAttrFilter) != 0)
			continue;

		if (lstrlen(lpFind->fd.cFileName) > lpFind->nSpaceLeft) 
		{
			if (!lpFind->fd.cAlternateFileName[0] ||
				lstrlen(lpFind->fd.cAlternateFileName) > lpFind->nSpaceLeft) 
			{
				continue;
			}
			lstrcpy(lpFind->fd.cFileName, lpFind->fd.cAlternateFileName);
		}

		Wow64RevertWow64FsRedirection(oldValue);
		lpFind->err = 0;
		return TRUE;
	}

	lpFind->err = GetLastError();
	Wow64RevertWow64FsRedirection(oldValue);
	return(FALSE);
}

BOOL WFFindClose(LPLFNDTA lpFind)
{
	BOOL bRet;

	if (lpFind->hFindFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	bRet = FindClose(lpFind->hFindFile);
	return bRet;
}

BOOL WFIsDir(LPTSTR lpDir)
{
	DWORD attr = GetFileAttributes(lpDir);

	if (attr == INVALID_FILE_ATTRIBUTES)
		return FALSE;

	if (attr & FILE_ATTRIBUTE_DIRECTORY)
		return TRUE;

	return FALSE;
}

WORD I_LFNEditName(LPTSTR lpSrc, LPTSTR lpEd, LPTSTR lpRes, INT iResBufSize)
{
	INT ResLen = 0;

#ifdef  USELASTDOT
	LPTSTR lpChar;

	lpChar = GetExtension(lpSrc);
	if (*lpChar)
	{
		lpChar--;
	}
	else
	{
		lpChar = NULL;
	}
#endif //  USELASTDOT

	while (*lpEd)
	{
		if (ResLen < iResBufSize)
		{
			switch (*lpEd)
			{
			case CHAR_STAR:
			{
				TCHAR delimit = *(lpEd + 1);

#ifdef USELASTDOT
				if (CHAR_DOT != delimit)
					lpChar = NULL;

				while ((ResLen < iResBufSize) && (*lpSrc != CHAR_NULL) && (*lpSrc != delimit || (lpChar && lpChar != lpSrc)))
				{
#else
				while ((ResLen < iResBufSize) &&
					(*lpSrc != CHAR_NULL) && (*lpSrc != delimit)) {
#endif // USELASTDOT

					*(lpRes++) = *(lpSrc++);
					ResLen++;
				}

			}
				break;
			case CHAR_QUESTION:
				if ((*lpSrc != CHAR_DOT) && (*lpSrc != CHAR_NULL))
				{
					if (ResLen < iResBufSize)
					{
						*(lpRes++) = *(lpSrc++);
						ResLen++;
					}
					else
						return ERROR_INVALID_PARAMETER;
				}
				break;
			case CHAR_DOT:
				while ((*lpSrc != CHAR_DOT) && (*lpSrc != CHAR_NULL))
				{
					lpSrc++;
				}

				*(lpRes++) = CHAR_DOT;

				ResLen++;

				if (*lpSrc)
					lpSrc++;
				break;
			default:
				if ((*lpSrc != CHAR_DOT) && (*lpSrc != CHAR_NULL))
				{
					lpSrc++;
				}

				if (ResLen < iResBufSize)
				{
					*(lpRes++) = *lpEd;
					ResLen++;
				}
				else
				{
					return ERROR_INVALID_PARAMETER;
				}
				break;
			}

			lpEd++;
		}
		else
		{
			return ERROR_INVALID_PARAMETER;
		}
	}

	if (ResLen < iResBufSize)
	{
		*lpRes = CHAR_NULL;
		return NO_ERROR;
	}
	else
	{
		return ERROR_INVALID_PARAMETER;
	}
}

BOOL LFNMergePath(LPTSTR lpMask, LPTSTR lpFile)
{
	TCHAR szT[MAX_PATH * 2];
	INT iResStrLen;

	lstrcpy(szT, lpMask);

	RemoveLast(szT);

	AddBackSlach(szT);

	if (!(CHAR_BACKSLASH == lpFile[0] && CHAR_NULL == lpFile[1]))
	{
		iResStrLen = lstrlen(szT);

		I_LFNEditName(lpFile, FindFileName(lpMask), szT + iResStrLen, COUNTOF(szT) - iResStrLen);

		iResStrLen = lstrlen(szT);
		if ((iResStrLen != 0) && CHAR_DOT == szT[iResStrLen - 1])
			szT[iResStrLen - 1] = CHAR_NULL;
	}

	lstrcpy(lpMask, szT);
	return TRUE;
}

DWORD WFCopy(LPTSTR pszFrom, LPTSTR pszTo)
{
	DWORD dwRet;
	TCHAR szTemp[MAX_PATH];

	Notify(hdlgProgress, IDS_COPYINGMSG, pszFrom, pszTo);  //TODO: invalid

	if (CopyFile(pszFrom, pszTo, FALSE))
	{
		ChangeFileSystem
	}


}

