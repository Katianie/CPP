#include <crtdbg.h>
#include <stdio.h>
#include <Windows.h>

#define ASSERT _ASSERTE

#ifdef _DEBUG
inline void Trace(const wchar_t* format, ...)
{
	wchar_t buffer[256];

	va_list args;
	va_start(args, format);
	ASSERT(_vsnwprintf_s(buffer, (sizeof(buffer) / sizeof(wchar_t)) - 1, format, args) != -1);
	va_end(args);

	OutputDebugStringW(buffer);
}
#endif // _DEBUG

#ifdef _DEBUG
#define VERIFY ASSERT
#define TRACE Trace
#else
#define VERIFY(expression) (expression)
#define TRACE __noop
#endif

struct LastException
{
	DWORD myErrorCode;

	LastException()
	{
		myErrorCode = GetLastError();
	}
};

struct ManualResetEvent
{
	HANDLE myHandle;

	ManualResetEvent()
	{
		myHandle = CreateEvent(NULL, true, false, NULL);

		if (myHandle == NULL)
		{
			throw LastException();
		}
	}

	~ManualResetEvent()
	{
		VERIFY(CloseHandle(myHandle));
	}
};

int main()
{
	//ManualResetEvent resetEvent = ManualResetEvent();
	//ManualResetEvent otherEvent = resetEvent;

	//static_assert(sizeof(float) == 2, "uhh big problem");

	TRACE(L"%S(%d):1 + 2 = %d\n",__FILEW__, __LINE__, 1 + 2);

	system("pause");
	return 0;
}
