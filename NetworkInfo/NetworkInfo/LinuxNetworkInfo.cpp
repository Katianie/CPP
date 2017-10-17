#include "LinuxNetworkInfo.h"

///Constructor
LinuxNetworkInfo::LinuxNetworkInfo() : NetworkInfo::NetworkInfo() //Call Base's Constructor.
{

}

///Destructor
LinuxNetworkInfo::~LinuxNetworkInfo()
{
	//Call base's Destructor.
	NetworkInfo::~NetworkInfo();
}

///Functions
char* LinuxNetworkInfo::GetInfo()
{
	char* retVal = NULL;

#ifdef linux

#endif //Linux

	return retVal;
}

bool LinuxNetworkInfo::WriteInfoToFile(const char* fileName)
{
	return false;
}