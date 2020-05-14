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