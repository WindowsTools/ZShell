#include"pch.h"
#include"core.h"
#include"wfcopy.h"
#include"lfn.h"

#include<ShlObj.h>
#include<CommCtrl.h>
#include<Ole2.h>

VOID ChangeFileSystem(register DWORD dwFunction, LPWSTR lpszFile, LPWSTR lpszTo)
{
	//cpu register first
	HWND hwnd, hwndTree, hwndOld;
	TCHAR szFrom[MAX_PATH];
	TCHAR szTo[MAX_PATH];
	TCHAR szTemp[MAX_PATH];
	TCHAR szPath[MAX_PATH];

	if (cDisableFSC == 0 || bFSCTimerSet)
	{
		if (bFSCTimerSet)
			KillTimer(hwndFrame, 1);

		if (SetTimer(hwndFrame, 1, 1000, NULL))
		{
			bFSCTimerSet = TRUE;
			if (cDisableFSC == 0)
				SendMessage(hwndFrame, FS_DISABLEFSC, 0, 0L);
		}
	}

	lstrcpy(szFrom, lpszFile);
	Qualifypath
}