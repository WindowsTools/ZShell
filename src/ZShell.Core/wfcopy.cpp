#include"pch.h"
#include"wfcopy.h"
#include"core.h"

UINT RemoveLast(LPTSTR pFile)
{
	LPTSTR pT;
	UINT uChars = 0;

	for (pT = pFile; *pFile; pFile++)
	{
		if (*pFile == CHAR_BACKSLASH)
		{
			pT = pFile;
			uChars = 0;
		}
		else if (*pFile == CHAR_COLON)
		{
			if (pFile[1] == CHAR_BACKSLASH)
			{
				pFile++;
			}

			pT = pFile + 1;

			uChars = 0;
			continue;
		}
		uChars++;
	}

	*pT = CHAR_NULL;
	return uChars;
}

LPTSTR FindFileName(LPTSTR pPath)
{
	LPTSTR pT;

	for (pT = pPath; *pPath; pPath++)
	{
		if ((pPath[0] == CHAR_BACKSLASH || pPath[0] == CHAR_COLON) && pPath[1])
		{
			pT = pPath + 1;
		}
	}

	return (pT);
}

VOID Notify(HWND hDlg, WORD idMessage, LPTSTR szFrom, LPTSTR szTo)
{
	TCHAR szTemp[40];

	if (idMessage) // idMessage = 1
	{
		//TODO: invalid
		LoadString(hAppInstance, idMessage, szTemp, COUNTOF(szTemp));
		SetDlgItemText(hDlg, IDD_STATUS, szTemp);
		SetDlgItemPath(hDlg, IDD_NAME, szFrom);
	}
	else
	{
		//TODO: invalid
		SetDlgItemText(hDlg, IDD_STATUS, szNULL);
		SetDlgItemText(hDlg, IDD_NAME, szNULL);
	}

	SetDlgItemPath(hDlg, IDD_TONAME, szTo);
}

VOID SetDlgItemPath(HWND hDlg, INT id, LPTSTR pszPath)
{
	RECT rc;
	HDC hdc;
	HFONT hFont;
	TCHAR szPath[MAX_PATH + 1];
	HWND hwnd;

	hwnd = GetDlgItem(hDlg, id);

	if (!hwnd)
		return;

	lstrcpy(szPath, pszPath);
	GetClientRect(hwnd, &rc);

	hdc = GetDC(hDlg);

	//todo to be continue
}

LPTSTR StripColon(register LPTSTR pPath)
{
	register INT cb = lstrlen(pPath);

	if (cb > 2 && pPath[cb - 1] == CHAR_COLON)
		pPath[cb - 1] = CHAR_NULL;

	return pPath;
}

BOOL QualifyPath(LPTSTR lpszPath)
{
	INT cb, nSpaceLeft, i, j;
	TCHAR szTemp[MAX_PATH];
	DRIVE drive = 0;
	LPTSTR pOrig, pT;
	BOOL flfn = FALSE;
	BOOL fQuote = FALSE;

	TCHAR szDrive[] = SZ_ACOLONSLASH;

	LPTSTR lpszDot;
	UINT uLen;

	lstrcpyn(szTemp, lpszPath, COUNTOF(szTemp));
	CheckSlashes(szTemp);
	StripColon(szTemp);

	nSpaceLeft = MAX_PATH - 1;

	for (pT = pOrig = szTemp;*pOrig;pOrig++)
	{
		if (*pOrig != CHAR_DQUOTE)
		{
			*pT++ = *pOrig;
		}
		else 
		{
			fQuote = TRUE;
		}
	}

	pOrig == szTemp;

	if (ISUNCPATH(pOrig))
	{
		for (i = 0, j = 2, pOrig += 2;*pOrig && i < 2;pOrig++, j++)
		{
			if (CHAR_BACKSLASH == *pOrig)
				i++;
		}

		if (!i)
			return FALSE;

		flfn = IsLFNDrive(lpszPath);
	}
}