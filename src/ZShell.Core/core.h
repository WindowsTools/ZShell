#pragma once

#include "resource.h"

#ifdef _GLOBALS
#define Extern
#define EQ(x) = x
#else
#define Extern extern
#define EQ(x)
#endif

#define CHAR_DASH TEXT('-')
#define CHAR_CARET TEXT('^')
#define CHAR_UNDERSCORE TEXT('_')
#define CHAR_AND TEXT('&')
#define CHAR_TAB TEXT('\t')
#define CHAR_LESS TEXT('<')
#define CHAR_GREATER TEXT('>')
#define CHAR_EQUAL TEXT('=')
#define CHAR_PLUS TEXT('+')
#define CHAR_SEMICOLON TEXT(';')
#define CHAR_COMMA TEXT(',')
#define CHAR_PIPE TEXT('|')
#define CHAR_BACKSLASH TEXT('\\')
#define CHAR_SLASH TEXT('/')
#define CHAR_OPENBRACK TEXT('[')
#define CHAR_CLOSEBRACK TEXT(']')
#define CHAR_ZERO TEXT('0')
#define CHAR_COLON TEXT(':')
#define CHAR_SPACE TEXT(' ')
#define CHAR_NEWLINE TEXT('\n')

#define CHAR_DOT TEXT('.')
#define CHAR_OPENPAREN TEXT('(')
#define CHAR_CLOSEPAREN TEXT(')')
#define CHAR_HASH TEXT('#')
#define CHAR_DQUOTE TEXT('"')

#define CHAR_NULL TEXT('\0')
#define CHAR_QUESTION TEXT('?')
#define CHAR_STAR TEXT('*')
#define CHAR_PERCENT TEXT('%')

#define SZ_ACOLONSLASH    TEXT("A:\\")

#define ATTR_USED           0x6DBF	

#define COUNTOF(x) (sizeof(x)/sizeof(*x))
#define ISUNCPATH(x) (CHAR_BACKSLASH == x[0] && CHAR_BACKSLASH == x[1])

Extern HINSTANCE  hAppInstance;  //TODO: invalid

Extern HWND    hdlgProgress;     //TODO: invalid
Extern HWND    hwndFrame       EQ(NULL);  //TODO: invalid

Extern TCHAR        szNULL[]                EQ(TEXT(""));

Extern INT   cDisableFSC    EQ(0);   

Extern BOOL bFSCTimerSet       EQ(FALSE);

#define FS_ENABLEFSC               (WM_USER+0x121)
#define FS_DISABLEFSC              (WM_USER+0x122)

typedef HWND* PHWND;
typedef INT DRIVE;
typedef INT DRIVEIND;

VOID ChangeFileSystem(DWORD dwOper, LPWSTR lpPath, LPWSTR lpTo);
BOOL  QualifyPath(LPTSTR);

//wfutil.cpp
VOID  CheckSlashes(LPTSTR);

//wfcopy.cpp
LPTSTR StripColon(LPTSTR pPath);