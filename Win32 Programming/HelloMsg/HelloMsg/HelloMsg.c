#include <windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iShowCmd)
{
	MessageBox(NULL, TEXT("Hello World"), TEXT("HelloMsg"), MB_YESNO | MB_ICONHAND);

	return 0;
}