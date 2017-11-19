#pragma once
#include "NetworkInfo.h"
#ifdef _WIN32 //If using Windows
#include "WindowsNetworkInfo.h"
#endif //Win32
#ifdef linux //If using Linux
#include "LinuxNetworkInfo.h"
#endif //Linux

using namespace std;

int main()
{
	NetworkInfo* networkInfo = NULL;

	//If using Windows.
#ifdef _WIN32 
	networkInfo = new WindowsNetworkInfo();
#endif

	//If using Linux.
#ifdef linux 
	networkInfo = new LinuxNetworkInfo();
#endif

	if (networkInfo != NULL)
	{
		cout << networkInfo->GetInfo() << endl;

		networkInfo->WriteInfoToFile();

		delete networkInfo;
	}

	system("pause");
	return 0;
}