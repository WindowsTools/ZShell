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
		}
	}
}