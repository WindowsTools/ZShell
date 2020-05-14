#include "pch.h"
#include "core.h"
#include "lfn.h"
#include "wfutil.h"
#include <CommCtrl.h>
#include <stdlib.h>

UINT AddBackSlach(LPTSTR lpszPath)
{
	register UINT uLen = lstrlen(lpszPath);

	if (*(lpszPath + uLen - 1) != CHAR_BACKSLASH)
	{
		lpszPath[uLen++] = CHAR_BACKSLASH;
		lpszPath[uLen] = CHAR_NULL;
	}

	return uLen;
}

LPTSTR GetExtension(LPTSTR pszFile)
{
	LPTSTR p, pSave = NULL;

	p = pszFile;


	while (*p)
	{
		if (*p == CHAR_DOT)
			pSave = p;
		p++;
	}

	if (!pSave)
		return p;

	return pSave + 1;
}

