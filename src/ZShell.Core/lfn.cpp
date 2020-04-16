#include "pch.h"
#include "lfn.h"
#include "core.h"

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