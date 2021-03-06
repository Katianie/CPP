#include <windows.h>
#include <tchar.h>
#include <stdio.h>

int CDECL MessageBoxPrintf(TCHAR * szCaption, TCHAR * szFormat, ...)
{
	TCHAR szBuffer[1024];
	va_list pArgList;

	va_start(pArgList, szFormat);

	_vsntprintf(szBuffer, sizeof(szBuffer) / sizeof(TCHAR), szFormat, pArgList);

	va_end(pArgList);

	return MessageBox(NULL, szBuffer, szCaption, 0);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	int xScreen;
	int yScreen;

	xScreen = GetSystemMetrics(SM_CXSCREEN);
	yScreen = GetSystemMetrics(SM_CYSCREEN);

	MessageBoxPrintf(_TEXT("Screen Size"), _TEXT("The Screen is %i pixles wide by %i pixles high."), xScreen, yScreen);

	return 0;
}

