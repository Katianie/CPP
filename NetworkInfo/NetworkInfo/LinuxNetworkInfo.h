#pragma once
#include "NetworkInfo.h"

class LinuxNetworkInfo : public NetworkInfo
{
public:
	///Constructor
	LinuxNetworkInfo();

	///Destructor
	virtual ~LinuxNetworkInfo();

	///Functions
	virtual char* GetInfo();
	virtual bool WriteInfoToFile(const char* fileName);
};