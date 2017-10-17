#pragma once
#include "NetworkInfo.h"

class WindowsNetworkInfo : public NetworkInfo
{

protected:
//#ifdef _WIN32
	unsigned int myNumAdapters;
	char** myAdapterNames;

	unsigned int* myNumAdapterPrefixAddresses;
	char*** myAdapterPrefixAddresses;

	unsigned int* myNumAnycastAddresses;
	char*** myAnycastAddresses;

	char** myConnectionTypes;

	char** myCompartmentTypes;

	wchar_t** myDescriptions;

	char** myDHCPV4Servers;

	char** myDHCPV6Servers;

	char** myDHCPV6ClientDUIDs;

	unsigned long* myDHCPV6Iads;

	unsigned int* myNumDNSServerAddresses;
	char*** myDNSServerAddresses;

	unsigned int* myNumDNSSuffixes;
	wchar_t*** myDNSSuffixes;

	char** myFlags;

	wchar_t** myFriendlyNames;

	char** myIfTypes;

	unsigned long* myIPV4IfIndexes;

	unsigned long* myIPV6IfIndexes;

	unsigned long* myIPV4Metrics;

	unsigned long* myIPV6Metrics;

	unsigned int* myNumGatewayAddresses;
	char*** myGatewayAddresses;

	unsigned __int64* myLUIDs;
	char** myLUIDIfTypes;
	unsigned __int64* myLUIDNetIndexes;

	unsigned long* myMTUs;

	unsigned int* myNumMulticastAddresses;
	char*** myMulticastAddresses;

	char** myNetworkGUIDs;

	char** myOperStatuses;

	unsigned int* myPhysicalAddressLengths;
	char*** myPhysicalAddresses;

	unsigned __int64* myReceiveLinkSpeeds;
	unsigned __int64* myTransmitLinkSpeeds;

	char** myTunnelTypes;

	unsigned int* myNumUnicastAddresses;
	char*** myUnicastAddresses;

	unsigned int* myNumWindowsServerAddresses;
	char*** myWindowsServerAddresses;

	unsigned int* myNumZoneIndexes;
	unsigned long** myZoneIndexes;

//#endif

	///Protected Functions
	virtual bool InitializeBuffers(unsigned int numAdapters);
	virtual void DestroyBuffers(unsigned int numAdapters);

public:
	///Constructor
	WindowsNetworkInfo();

	///Destructor
	virtual ~WindowsNetworkInfo();

	///Functions
	virtual void Initialize();
	virtual const char* GetIfTypeString(unsigned long ifType);
	virtual char* GetInfo();
	virtual bool WriteInfoToFile(const char* fileName);
};