#pragma once
#include "NetworkInfo.h"

class WindowsNetworkInfo : public NetworkInfo
{
protected:
	unsigned int myNumAdapters;
	unsigned int* myNumAdapterPrefixAddresses;
	unsigned int* myNumAnycastAddresses;
	unsigned int* myNumDNSServerAddresses;
	unsigned int* myNumDNSSuffixes;
	unsigned int* myNumGatewayAddresses;
	unsigned int* myNumMulticastAddresses;
	unsigned int* myPhysicalAddressLengths;
	unsigned int* myNumUnicastAddresses;
	unsigned int* myNumWindowsServerAddresses;

	char**				myAdapterNames;
	char***				myAdapterPrefixAddresses;
	char***				myAnycastAddresses;
	char**				myConnectionTypes;
	char**				myCompartmentTypes;
	wchar_t**			myDescriptions;
	char**				myDHCPV4Servers;
	char**				myDHCPV6Servers;
	char**				myDHCPV6ClientDUIDs;
	unsigned long*		myDHCPV6Iads;
	char***				myDNSServerAddresses;
	wchar_t***			myDNSSuffixes;
	char**				myFlags;
	wchar_t**			myFriendlyNames;
	char**				myIfTypes;
	unsigned long*		myIPV4IfIndexes;
	unsigned long*		myIPV6IfIndexes;
	unsigned long*		myIPV4Metrics;
	unsigned long*		myIPV6Metrics;
	char***				myGatewayAddresses;
	unsigned __int64*	myLUIDs;
	char**				myLUIDIfTypes;
	unsigned __int64*	myLUIDNetIndexes;
	unsigned long*		myMTUs;
	char***				myMulticastAddresses;
	char**				myNetworkGUIDs;
	char**				myOperStatuses;
	char**				myPhysicalAddresses;
	unsigned __int64*	myReceiveLinkSpeeds;
	unsigned __int64*	myTransmitLinkSpeeds;
	char**				myTunnelTypes;
	char***				myUnicastAddresses;
	char***				myWindowsServerAddresses;
	unsigned long**		myZoneIndexes;

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