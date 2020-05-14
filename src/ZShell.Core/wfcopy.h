#pragma once

UINT RemoveLast(LPTSTR pFile);
LPTSTR FindFileName(LPTSTR pPath);
VOID Notify(HWND hDlg, WORD iMessage, LPTSTR szFrom, LPTSTR szTo);
VOID SetDlgItemPath(HWND hDlg, INT id, LPTSTR pszPath);