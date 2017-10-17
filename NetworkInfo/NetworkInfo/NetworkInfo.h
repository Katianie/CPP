#pragma once

//If using Windows
#ifdef _WIN32
#include <stdio.h>
#include <iostream>
#include <winsock2.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iphlpapi.h>
#include <Windows.h>
#include <wchar.h>
#include <string>
// Link with Iphlpapi.lib
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "IPHLPAPI.lib")
#endif //Win32

//If using Linux
#ifdef linux
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <ifaddrs.h>
#endif //Linux

//Abstract base class of the Windows/Linux Network Info classes.
class NetworkInfo
{
public:
	///Constants.
	const unsigned int MAX_SUB_ARRAY_SIZE = 2 << 7;
	const unsigned int MAX_CONNECTION_ATTEMPTS = 2 << 1;

	///Constructor
	NetworkInfo();

	///Destructor
	virtual ~NetworkInfo();

	///Functions
	virtual char* GetInfo() = 0;
	virtual bool WriteInfoToFile(const char* fileName = "NetworkInfo.txt") = 0;
};
