#include "WindowsNetworkInfo.h"

///Constructor
WindowsNetworkInfo::WindowsNetworkInfo() : NetworkInfo::NetworkInfo() //Call Base's Constructor.
{
	WindowsNetworkInfo::Initialize();
}

///Destructor
WindowsNetworkInfo::~WindowsNetworkInfo()
{
	//Call base's destructor.
	NetworkInfo::~NetworkInfo();

	//Destroy any and all allocated data buffers.
	WindowsNetworkInfo::DestroyBuffers(myNumAdapters);
}

///Functions
void WindowsNetworkInfo::Initialize()
{
#ifdef _WIN32
	IP_ADAPTER_ADDRESSES* pAdapterAddresses = NULL;
	IP_ADAPTER_PREFIX* pAdapterPrefixes = NULL;
	IP_ADAPTER_ANYCAST_ADDRESS* pAnycastAddresses = NULL;
	IP_ADAPTER_DNS_SERVER_ADDRESS* pDNSServerAddresses = NULL;
	IP_ADAPTER_DNS_SUFFIX* pDNSSuffixes = NULL;
	IP_ADAPTER_GATEWAY_ADDRESS* pGatewayAddresses = NULL;
	IP_ADAPTER_MULTICAST_ADDRESS* pMulticastAddresses = NULL;
	IP_ADAPTER_UNICAST_ADDRESS* pUnicastAddresses = NULL;
	IP_ADAPTER_WINS_SERVER_ADDRESS* pWindowsServerAddresses = NULL;

	DWORD adaptersRetVal = ERROR_BUFFER_OVERFLOW;
	ULONG adaptersFlag = GAA_FLAG_INCLUDE_PREFIX;
	ULONG adapterBufferLength = 0;

	sockaddr_in* sockaddressIn = NULL;
	sockaddr_in6* sockaddressIn6 = NULL;

	//Attempt to connect multiple times.
	for (unsigned int connectionAttempt = 0; connectionAttempt <= MAX_CONNECTION_ATTEMPTS && adaptersRetVal != NO_ERROR; connectionAttempt++)
	{
		//First attempt will fail but it tells us how big this buffer actually needs to be.
		pAdapterAddresses = (IP_ADAPTER_ADDRESSES*)malloc(adapterBufferLength);
		if (pAdapterAddresses == NULL)
		{
			throw("Fatal Error: Memory allocation failed for IP_ADAPTER_ADDRESSES!\n");
		}
		else
		{
			//AF_UNSPEC returns both IPV4 and IPV6.
			adaptersRetVal = GetAdaptersAddresses(AF_UNSPEC, adaptersFlag, NULL, pAdapterAddresses, &adapterBufferLength);

			if (adaptersRetVal != NO_ERROR)
			{
				if (connectionAttempt > 0)
				{
					printf("Error: Failed to get adapters addresses with return code %d (attempt %u/%d)\n.", adaptersRetVal, connectionAttempt, MAX_CONNECTION_ATTEMPTS);
				}

				delete pAdapterAddresses;
				pAdapterAddresses = NULL;
			}
		}
	}

	//Ensure we connected successfully.
	IP_ADAPTER_ADDRESSES* headAdapterAddress = pAdapterAddresses;
	if (adaptersRetVal == NO_ERROR)
	{
		//Calculate number of adapters.
		for (myNumAdapters = 0; headAdapterAddress != NULL; myNumAdapters++)
		{
			headAdapterAddress = headAdapterAddress->Next;
		}
	}

	//Ensure we found at least one adapter.
	if (myNumAdapters > 0)
	{
		//Ensure the buffers initialized successfully before attempting to get the data.
		if (InitializeBuffers(myNumAdapters) == true)
		{
			//Hold on to the head so we can delete later.
			headAdapterAddress = pAdapterAddresses;

			//Go through each adapter we found.
			for (unsigned int currAdapterIndex = 0; pAdapterAddresses != NULL && currAdapterIndex < myNumAdapters; currAdapterIndex++)
			{
				//Adapter Name
				if (pAdapterAddresses->AdapterName != NULL && strlen(pAdapterAddresses->AdapterName) > 0)
				{
					size_t adapterNameLen = strlen(pAdapterAddresses->AdapterName) + 1;
					if (adapterNameLen > 1)
					{
						myAdapterNames[currAdapterIndex] = (char*)calloc(adapterNameLen, sizeof(char));
						if (strcpy_s(myAdapterNames[currAdapterIndex], adapterNameLen * sizeof(char), pAdapterAddresses->AdapterName) != 0)
						{
							printf("Error: Failed to get Adapter Name[%u]\n", currAdapterIndex);
							free(myAdapterNames[currAdapterIndex]);
							myAdapterNames[currAdapterIndex] = NULL;
						}
					}
				}
				else
				{
					myAdapterNames[currAdapterIndex] = NULL;
				}

				//Adapter Prefixes
				pAdapterPrefixes = pAdapterAddresses->FirstPrefix;
				if (pAdapterPrefixes != NULL)
				{
					//Allocate array for each adapter's prefixes (up to 256 prefixes).
					myNumAdapterPrefixAddresses[currAdapterIndex] = 0;
					myAdapterPrefixAddresses[currAdapterIndex] = (char**)malloc(NetworkInfo::MAX_SUB_ARRAY_SIZE * sizeof(char*));

					while (pAdapterPrefixes != NULL)
					{
						SOCKET_ADDRESS currPrefixAddress = pAdapterPrefixes->Address;
						if (currPrefixAddress.lpSockaddr != NULL)
						{
							if (currPrefixAddress.lpSockaddr->sa_family == AF_INET)
							{
								//Allocate buffer space to hold the IPs.
								myAdapterPrefixAddresses[currAdapterIndex][myNumAdapterPrefixAddresses[currAdapterIndex]] = (char*)calloc(INET_ADDRSTRLEN, sizeof(char));

								//Get the String representation of the IPV4 address.
								sockaddressIn = (sockaddr_in*)currPrefixAddress.lpSockaddr;
								inet_ntop(AF_INET, &(sockaddressIn->sin_addr), myAdapterPrefixAddresses[currAdapterIndex][myNumAdapterPrefixAddresses[currAdapterIndex]], INET_ADDRSTRLEN);
								myNumAdapterPrefixAddresses[currAdapterIndex]++;
							}
							else if (currPrefixAddress.lpSockaddr->sa_family == AF_INET6)
							{
								//Allocate buffer space to hold the IPs.
								myAdapterPrefixAddresses[currAdapterIndex][myNumAdapterPrefixAddresses[currAdapterIndex]] = (char*)calloc(INET6_ADDRSTRLEN, sizeof(char));

								//Get the String representation of the IPV6 address.
								sockaddressIn6 = (sockaddr_in6*)currPrefixAddress.lpSockaddr;
								inet_ntop(AF_INET6, &(sockaddressIn6->sin6_addr), myAdapterPrefixAddresses[currAdapterIndex][myNumAdapterPrefixAddresses[currAdapterIndex]], INET6_ADDRSTRLEN);
								myNumAdapterPrefixAddresses[currAdapterIndex]++;
							}
							else
							{
								//Nether IPV4 or IPV6.
								myAdapterPrefixAddresses[currAdapterIndex][myNumAdapterPrefixAddresses[currAdapterIndex]] = NULL;
							}
						}

						pAdapterPrefixes = pAdapterPrefixes->Next;
					}

					//Check if nothing was found.
					if (myNumAdapterPrefixAddresses <= 0)
					{
						free(myAdapterPrefixAddresses[currAdapterIndex]);
						myAdapterPrefixAddresses[currAdapterIndex] = NULL;
					}
				}
				else
				{
					myNumAdapterPrefixAddresses[currAdapterIndex] = 0;
					myAdapterPrefixAddresses[currAdapterIndex] = NULL;
				}

				//Anycast Addresses
				pAnycastAddresses = pAdapterAddresses->FirstAnycastAddress;
				if (pAnycastAddresses != NULL)
				{
					//Allocate array for each adapter's prefixes (up to 256 prefixes).
					myNumAnycastAddresses[currAdapterIndex] = 0;
					myAnycastAddresses[currAdapterIndex] = (char**)malloc(NetworkInfo::MAX_SUB_ARRAY_SIZE * sizeof(char*));

					while (pAnycastAddresses != NULL)
					{
						SOCKET_ADDRESS currAnycastAddress = pAnycastAddresses->Address;
						if (currAnycastAddress.lpSockaddr != NULL)
						{
							if (currAnycastAddress.lpSockaddr->sa_family == AF_INET)
							{
								//Allocate buffer space to hold the IPs.
								myAnycastAddresses[currAdapterIndex][myNumAnycastAddresses[currAdapterIndex]] = (char*)calloc(INET_ADDRSTRLEN, sizeof(char));

								//Get the String representation of the IPV4 address.
								sockaddressIn = (sockaddr_in*)currAnycastAddress.lpSockaddr;
								inet_ntop(AF_INET, &(sockaddressIn->sin_addr), myAnycastAddresses[currAdapterIndex][myNumAnycastAddresses[currAdapterIndex]], INET_ADDRSTRLEN);
								myNumAnycastAddresses[currAdapterIndex]++;
							}
							else if (currAnycastAddress.lpSockaddr->sa_family == AF_INET6)
							{
								//Allocate buffer space to hold the IPs.
								myAnycastAddresses[currAdapterIndex][myNumAnycastAddresses[currAdapterIndex]] = (char*)calloc(INET6_ADDRSTRLEN, sizeof(char));

								//Get the String representation of the IPV6 address.
								sockaddressIn6 = (sockaddr_in6*)currAnycastAddress.lpSockaddr;
								inet_ntop(AF_INET6, &(sockaddressIn6->sin6_addr), myAnycastAddresses[currAdapterIndex][myNumAnycastAddresses[currAdapterIndex]], INET6_ADDRSTRLEN);
								myNumAnycastAddresses[currAdapterIndex]++;
							}
							else
							{
								//Nether IPV4 or IPV6.
								myAnycastAddresses[currAdapterIndex][myNumAnycastAddresses[currAdapterIndex]] = NULL;
							}
						}

						pAnycastAddresses = pAnycastAddresses->Next;
					}

					//Check if nothing was found.
					if (myNumAnycastAddresses[currAdapterIndex] <= 0)
					{
						free(myAnycastAddresses[currAdapterIndex]);
						myAnycastAddresses[currAdapterIndex] = NULL;
					}
				}
				else
				{
					myNumAnycastAddresses[currAdapterIndex] = 0;
					myAnycastAddresses[currAdapterIndex] = NULL;
				}

				//Connection Type
				NET_IF_CONNECTION_TYPE currConnectionType = pAdapterAddresses->ConnectionType;
				size_t connectionTypeStrLen;
				char* connectionType = NULL;
				if (currConnectionType == NET_IF_CONNECTION_TYPE::NET_IF_CONNECTION_DEDICATED)
				{
					connectionType = "Connection Dedicated.";
				}
				else if (currConnectionType == NET_IF_CONNECTION_TYPE::NET_IF_CONNECTION_DEMAND)
				{
					connectionType = "Connection Demand.";
				}
				else if (currConnectionType == NET_IF_CONNECTION_TYPE::NET_IF_CONNECTION_MAXIMUM)
				{
					connectionType = "Connection Maximum.";
				}
				else if (currConnectionType == NET_IF_CONNECTION_TYPE::NET_IF_CONNECTION_PASSIVE)
				{
					connectionType = "Connection Passive.";
				}
				connectionTypeStrLen = strlen(connectionType) + 1;
				myConnectionTypes[currAdapterIndex] = (char*)calloc(connectionTypeStrLen, sizeof(char));
				if (strcpy_s(myConnectionTypes[currAdapterIndex], connectionTypeStrLen, connectionType) != 0)
				{
					printf("Error: Failed to get Connection Type[%u]", currAdapterIndex);
					free(myConnectionTypes[currAdapterIndex]);
					myConnectionTypes[currAdapterIndex] = NULL;
				}

				//Compartment Id
				NET_IF_COMPARTMENT_ID compartmentId = pAdapterAddresses->CompartmentId;
				if (compartmentId > 0)
				{
					size_t compartmentTypeStrLen;
					char* compartmentType = NULL;
					if (compartmentId == NET_IF_COMPARTMENT_ID_UNSPECIFIED)
					{
						compartmentType = "Compartment Unspecified.";
					}
					else if (compartmentId == NET_IF_COMPARTMENT_ID_PRIMARY)
					{
						compartmentType = "Compartment Primary.";
					}
					else if (compartmentId == NET_IF_COMPARTMENT_ID_ALL)
					{
						compartmentType = "Compartment All.";
					}
					compartmentTypeStrLen = strlen(compartmentType) + 1;
					myCompartmentTypes[currAdapterIndex] = (char*)calloc(compartmentTypeStrLen, sizeof(char));
					if (strcpy_s(myCompartmentTypes[currAdapterIndex], compartmentTypeStrLen, compartmentType) != 0)
					{
						printf("Error: Failed to get Compartment Type[%u]", currAdapterIndex);
						free(myCompartmentTypes[currAdapterIndex]);
						myCompartmentTypes[currAdapterIndex] = NULL;
					}
				}

				//Description
				if (pAdapterAddresses->Description != NULL)
				{
					size_t descriptionStrLen = wcslen(pAdapterAddresses->Description) + 2;
					if (descriptionStrLen > 2)
					{
						myDescriptions[currAdapterIndex] = (wchar_t*)calloc(descriptionStrLen, sizeof(wchar_t));
						if (wcsncpy_s(myDescriptions[currAdapterIndex], descriptionStrLen, pAdapterAddresses->Description, descriptionStrLen) != 0)
						{
							printf("Error: Failed to get Description[%u]", currAdapterIndex);
							free(myDescriptions[currAdapterIndex]);
							myDescriptions[currAdapterIndex] = NULL;
						}
					}
					else
					{
						myDescriptions[currAdapterIndex] = NULL;
					}
				}

				//DHCP V4 Server
				myDHCPV4Servers[currAdapterIndex] = NULL;
				SOCKET_ADDRESS dhcpv4Server = pAdapterAddresses->Dhcpv4Server;
				if (dhcpv4Server.lpSockaddr != NULL)
				{
					if (dhcpv4Server.lpSockaddr->sa_family == AF_INET)
					{
						myDHCPV4Servers[currAdapterIndex] = (char*)calloc(INET_ADDRSTRLEN + 1, sizeof(char));

						//Get the String representation of the IPV4 address.
						sockaddressIn = (sockaddr_in*)dhcpv4Server.lpSockaddr;
						inet_ntop(AF_INET, &(sockaddressIn->sin_addr), myDHCPV4Servers[currAdapterIndex], INET_ADDRSTRLEN + 1);
					}
					else if (dhcpv4Server.lpSockaddr->sa_family == AF_INET6)
					{
						myDHCPV4Servers[currAdapterIndex] = (char*)calloc(INET6_ADDRSTRLEN + 1, sizeof(char));

						//Get the String representation of the IPV6 address.
						sockaddressIn6 = (sockaddr_in6*)dhcpv4Server.lpSockaddr;
						inet_ntop(AF_INET6, &(sockaddressIn6->sin6_addr), myDHCPV4Servers[currAdapterIndex], INET6_ADDRSTRLEN + 1);
					}
				}

				//DHCP V6 Server
				myDHCPV6Servers[currAdapterIndex] = NULL;
				SOCKET_ADDRESS dhcpv6Server = pAdapterAddresses->Dhcpv6Server;
				if (dhcpv6Server.lpSockaddr != NULL)
				{
					if (dhcpv6Server.lpSockaddr->sa_family == AF_INET)
					{
						myDHCPV6Servers[currAdapterIndex] = (char*)calloc(INET_ADDRSTRLEN + 1, sizeof(char));

						//Get the String representation of the IPV4 address.
						sockaddressIn = (sockaddr_in*)dhcpv6Server.lpSockaddr;
						inet_ntop(AF_INET, &(sockaddressIn->sin_addr), myDHCPV6Servers[currAdapterIndex], INET_ADDRSTRLEN + 1);
					}
					else if (dhcpv6Server.lpSockaddr->sa_family == AF_INET6)
					{
						myDHCPV6Servers[currAdapterIndex] = (char*)calloc(INET6_ADDRSTRLEN + 1, sizeof(char));

						//Get the String representation of the IPV6 address.
						sockaddressIn6 = (sockaddr_in6*)dhcpv6Server.lpSockaddr;
						inet_ntop(AF_INET6, &(sockaddressIn6->sin6_addr), myDHCPV6Servers[currAdapterIndex], INET6_ADDRSTRLEN + 1);
					}
				}

				//DHCP V6 Client DUID
				ULONG dhcpv6ClientDUIDLen = pAdapterAddresses->Dhcpv6ClientDuidLength;
				if (dhcpv6ClientDUIDLen > 0)
				{
					myDHCPV6ClientDUIDs[currAdapterIndex] = (char*)calloc(dhcpv6ClientDUIDLen + 1, sizeof(char));
					for (unsigned long currDHCPCLientDUIDIndex = 0; currDHCPCLientDUIDIndex < dhcpv6ClientDUIDLen; currDHCPCLientDUIDIndex++)
					{
						myDHCPV6ClientDUIDs[currAdapterIndex][currDHCPCLientDUIDIndex] = pAdapterAddresses->Dhcpv6ClientDuid[currDHCPCLientDUIDIndex];
					}
				}
				else
				{
					myDHCPV6ClientDUIDs[currAdapterIndex] = NULL;
				}

				//DHCP V6 IAID
				myDHCPV6Iads[currAdapterIndex] = pAdapterAddresses->Dhcpv6Iaid;

				//DNS Server Addresses.
				pDNSServerAddresses = pAdapterAddresses->FirstDnsServerAddress;
				if (pDNSServerAddresses != NULL)
				{
					//Allocate array for each adapter's prefixes (up to 256 prefixes).
					myNumDNSServerAddresses[currAdapterIndex] = 0;
					myDNSServerAddresses[currAdapterIndex] = (char**)malloc(NetworkInfo::MAX_SUB_ARRAY_SIZE * sizeof(char*));

					while (pDNSServerAddresses != NULL)
					{
						SOCKET_ADDRESS currDNSServerAddress = pDNSServerAddresses->Address;
						if (currDNSServerAddress.lpSockaddr != NULL)
						{
							if (currDNSServerAddress.lpSockaddr->sa_family == AF_INET)
							{
								//Allocate buffer space to hold the IPs.
								myDNSServerAddresses[currAdapterIndex][myNumDNSServerAddresses[currAdapterIndex]] = (char*)calloc(INET_ADDRSTRLEN, sizeof(char));

								//Get the String representation of the IPV4 address.
								sockaddressIn = (sockaddr_in*)currDNSServerAddress.lpSockaddr;
								inet_ntop(AF_INET, &(sockaddressIn->sin_addr), myDNSServerAddresses[currAdapterIndex][myNumDNSServerAddresses[currAdapterIndex]], INET_ADDRSTRLEN);
								myNumDNSServerAddresses[currAdapterIndex]++;
							}
							else if (currDNSServerAddress.lpSockaddr->sa_family == AF_INET6)
							{
								//Allocate buffer space to hold the IPs.
								myDNSServerAddresses[currAdapterIndex][myNumDNSServerAddresses[currAdapterIndex]] = (char*)calloc(INET6_ADDRSTRLEN, sizeof(char));

								//Get the String representation of the IPV6 address.
								sockaddressIn6 = (sockaddr_in6*)currDNSServerAddress.lpSockaddr;
								inet_ntop(AF_INET6, &(sockaddressIn6->sin6_addr), myDNSServerAddresses[currAdapterIndex][myNumDNSServerAddresses[currAdapterIndex]], INET6_ADDRSTRLEN);
								myNumDNSServerAddresses[currAdapterIndex]++;
							}
							else
							{
								//Nether IPV4 or IPV6.
								myDNSServerAddresses[currAdapterIndex][myNumDNSServerAddresses[currAdapterIndex]] = NULL;
							}
						}

						pDNSServerAddresses = pDNSServerAddresses->Next;
					}

					//Check if nothing was found.
					if (myNumDNSServerAddresses[currAdapterIndex] <= 0)
					{
						free(myDNSServerAddresses[currAdapterIndex]);
						myDNSServerAddresses[currAdapterIndex] = NULL;
					}
				}
				else
				{
					myNumDNSServerAddresses[currAdapterIndex] = 0;
					myDNSServerAddresses[currAdapterIndex] = NULL;
				}

				//DNS Suffix
				myNumDNSSuffixes[currAdapterIndex] = 0;
				pDNSSuffixes = pAdapterAddresses->FirstDnsSuffix;
				if (pDNSSuffixes != NULL)
				{
					//The first DNS suffix is stored separately so just make it the first in the array.
					size_t dnsSuffixStrLen = wcslen(pAdapterAddresses->DnsSuffix) + 2;
					if (dnsSuffixStrLen > 2)
					{
						myDNSSuffixes[currAdapterIndex] = (wchar_t**)malloc(MAX_SUB_ARRAY_SIZE * sizeof(wchar_t*));
						myDNSSuffixes[currAdapterIndex][0] = (wchar_t*)calloc(dnsSuffixStrLen, sizeof(wchar_t));
						if (wcsncpy_s(myDNSSuffixes[currAdapterIndex][0], dnsSuffixStrLen, pAdapterAddresses->DnsSuffix, dnsSuffixStrLen) != 0)
						{
							printf("Error: Failed to get DNS Suffix[%u]", currAdapterIndex);
							free(myDNSSuffixes[currAdapterIndex][0]);
							free(myDNSSuffixes[currAdapterIndex]);
							myDNSSuffixes[currAdapterIndex] = NULL;
						}
						else
						{
							//We know at least one DNS Suffix exists.
							myNumDNSSuffixes[currAdapterIndex] = 1;

							//Go through the list of additional DNS Suffixes.
							pDNSSuffixes = pAdapterAddresses->FirstDnsSuffix;
							while (pDNSSuffixes != NULL)
							{
								dnsSuffixStrLen = wcslen(pDNSSuffixes->String) + 2;

								if (dnsSuffixStrLen > 2)
								{
									myNumDNSSuffixes[currAdapterIndex]++;
									myDNSSuffixes[currAdapterIndex][myNumDNSSuffixes[currAdapterIndex]] = (wchar_t*)calloc(dnsSuffixStrLen, sizeof(wchar_t));
									if (wcsncpy_s(myDNSSuffixes[currAdapterIndex][myNumDNSSuffixes[currAdapterIndex]], dnsSuffixStrLen, pDNSSuffixes->String, dnsSuffixStrLen) != 0)
									{
										printf("Error: Failed to get DNS Suffix[%u]", currAdapterIndex);
										free(myDNSSuffixes[currAdapterIndex][myNumDNSSuffixes[currAdapterIndex]]);
										myDNSSuffixes[currAdapterIndex][myNumDNSSuffixes[currAdapterIndex]] = NULL;
									}
								}

								pDNSSuffixes = pDNSSuffixes->Next;
							}
						}
					}
				}
				else
				{
					myNumDNSSuffixes[currAdapterIndex] = 0;
					myDNSSuffixes[currAdapterIndex] = NULL;
				}

				//Determine Adapter Flags being used.
				ULONG flags = pAdapterAddresses->Flags;
				if (flags > 0)
				{
					size_t flagsBufferSize = 512;
					myFlags[currAdapterIndex] = (char*)calloc(flagsBufferSize, sizeof(char));
					if ((flags & IP_ADAPTER_DDNS_ENABLED) > 0)
					{
						//IP_ADAPTER_DDNS_ENABLED 0x0001
						if (sprintf_s(myFlags[currAdapterIndex], flagsBufferSize, "%s\n%s", myFlags[currAdapterIndex], "IP_ADAPTER_DDNS_ENABLED") == -1)
						{
							printf("Error: Failed string concatenation for flags[%u]", currAdapterIndex);
						}
					}
					if ((flags & IP_ADAPTER_REGISTER_ADAPTER_SUFFIX) > 0)
					{
						//IP_ADAPTER_REGISTER_ADAPTER_SUFFIX 0x0002
						if (sprintf_s(myFlags[currAdapterIndex], flagsBufferSize, "%s\n%s", myFlags[currAdapterIndex], "IP_ADAPTER_REGISTER_ADAPTER_SUFFIX") == -1)
						{
							printf("Error: Failed string concatenation for flags[%u]", currAdapterIndex);
						}
					}
					if ((flags & IP_ADAPTER_DHCP_ENABLED) > 0)
					{
						//IP_ADAPTER_DHCP_ENABLED 0x0004
						if (sprintf_s(myFlags[currAdapterIndex], flagsBufferSize, "%s\n%s", myFlags[currAdapterIndex], "IP_ADAPTER_DHCP_ENABLED") == -1)
						{
							printf("Error: Failed string concatenation for flags[%u]", currAdapterIndex);
						}
					}
					if ((flags & IP_ADAPTER_RECEIVE_ONLY) > 0)
					{
						//IP_ADAPTER_RECEIVE_ONLY 0x0008
						if (sprintf_s(myFlags[currAdapterIndex], flagsBufferSize, "%s\n%s", myFlags[currAdapterIndex], "IP_ADAPTER_RECEIVE_ONLY") == -1)
						{
							printf("Error: Failed string concatenation for flags[%u]", currAdapterIndex);
						}
					}
					if ((flags & IP_ADAPTER_NO_MULTICAST) > 0)
					{
						//IP_ADAPTER_NO_MULTICAST 0x0010
						if (sprintf_s(myFlags[currAdapterIndex], flagsBufferSize, "%s\n%s", myFlags[currAdapterIndex], "IP_ADAPTER_NO_MULTICAST") == -1)
						{
							printf("Error: Failed string concatenation for flags[%u]", currAdapterIndex);
						}
					}
					if ((flags & IP_ADAPTER_IPV6_OTHER_STATEFUL_CONFIG) > 0)
					{
						//IP_ADAPTER_IPV6_OTHER_STATEFUL_CONFIG 0x0020
						if (sprintf_s(myFlags[currAdapterIndex], flagsBufferSize, "%s\n%s", myFlags[currAdapterIndex], "IP_ADAPTER_IPV6_OTHER_STATEFUL_CONFIG") == -1)
						{
							printf("Error: Failed string concatenation for flags[%u]", currAdapterIndex);
						}
					}
					if ((flags & IP_ADAPTER_NETBIOS_OVER_TCPIP_ENABLED) > 0)
					{
						//IP_ADAPTER_NETBIOS_OVER_TCPIP_ENABLED 0x0040
						if (sprintf_s(myFlags[currAdapterIndex], flagsBufferSize, "%s\n%s", myFlags[currAdapterIndex], "IP_ADAPTER_NETBIOS_OVER_TCPIP_ENABLED") == -1)
						{
							printf("Error: Failed string concatenation for flags[%u]", currAdapterIndex);
						}
					}
					if ((flags & IP_ADAPTER_IPV4_ENABLED) > 0)
					{
						//IP_ADAPTER_IPV4_ENABLED 0x0080
						if (sprintf_s(myFlags[currAdapterIndex], flagsBufferSize, "%s\n%s", myFlags[currAdapterIndex], "IP_ADAPTER_IPV4_ENABLED") == -1)
						{
							printf("Error: Failed string concatenation for flags[%u]", currAdapterIndex);
						}
					}
					if ((flags & IP_ADAPTER_IPV6_ENABLED) > 0)
					{
						//IP_ADAPTER_IPV6_ENABLED 0x0100
						if (sprintf_s(myFlags[currAdapterIndex], flagsBufferSize, "%s\n%s", myFlags[currAdapterIndex], "IP_ADAPTER_IPV6_ENABLED") == -1)
						{
							printf("Error: Failed string concatenation for flags[%u]", currAdapterIndex);
						}
					}
					if ((flags & IP_ADAPTER_IPV6_MANAGE_ADDRESS_CONFIG) > 0)
					{
						//IP_ADAPTER_IPV6_MANAGE_ADDRESS_CONFIG 0x0200
						if (sprintf_s(myFlags[currAdapterIndex], flagsBufferSize, "%s\n%s", myFlags[currAdapterIndex], "IP_ADAPTER_IPV6_MANAGE_ADDRESS_CONFIG") == -1)
						{
							printf("Error: Failed string concatenation for flags[%u]", currAdapterIndex);
						}
					}
				}
				else
				{
					myFlags[currAdapterIndex] = NULL;
				}

				//Friendly Name
				size_t friendlyNameStrLen = wcslen(pAdapterAddresses->FriendlyName) + 2;
				if (friendlyNameStrLen > 2)
				{
					myFriendlyNames[currAdapterIndex] = (wchar_t*)calloc(friendlyNameStrLen, sizeof(wchar_t));
					if (wcsncpy_s(myFriendlyNames[currAdapterIndex], friendlyNameStrLen, pAdapterAddresses->FriendlyName, friendlyNameStrLen) == -1)
					{
						printf("Error: Failed to get Description[%u]", currAdapterIndex);
						free(myFriendlyNames[currAdapterIndex]);
						myFriendlyNames[currAdapterIndex] = NULL;
					}
				}
				else
				{
					myFriendlyNames[currAdapterIndex] = NULL;
				}

				//IfType
				const char* ifTypeStr = GetIfTypeString(pAdapterAddresses->IfType);
				size_t ifTypeStrLen = strlen(ifTypeStr) + 1;
				if (ifTypeStrLen > 1)
				{
					myIfTypes[currAdapterIndex] = (char*)calloc(ifTypeStrLen, sizeof(char));
					if (strcpy_s(myIfTypes[currAdapterIndex], ifTypeStrLen, ifTypeStr) != 0)
					{
						printf("Error: Failed to get IfType[%u]", currAdapterIndex);
						free(myIfTypes[currAdapterIndex]);
						myIfTypes[currAdapterIndex] = NULL;
					}
				}
				else
				{
					myIfTypes[currAdapterIndex] = NULL;
				}

				//IPv4 IfIndex
				myIPV4IfIndexes[currAdapterIndex] = pAdapterAddresses->IfIndex;

				//IPv6 IfIndex
				myIPV6IfIndexes[currAdapterIndex] = pAdapterAddresses->Ipv6IfIndex;

				//IPv4 Metric
				myIPV4Metrics[currAdapterIndex] = pAdapterAddresses->Ipv4Metric;

				//IPv6 Metric
				myIPV6Metrics[currAdapterIndex] = pAdapterAddresses->Ipv6Metric;

				//Gateway Addresses
				myNumGatewayAddresses[currAdapterIndex] = 0;
				pGatewayAddresses = pAdapterAddresses->FirstGatewayAddress;
				if (pGatewayAddresses != NULL)
				{
					//Allocate array for each adapter's prefixes (up to 256 prefixes).
					myGatewayAddresses[currAdapterIndex] = (char**)malloc(NetworkInfo::MAX_SUB_ARRAY_SIZE * sizeof(char*));

					while (pGatewayAddresses != NULL)
					{
						SOCKET_ADDRESS currGatewayAddress = pGatewayAddresses->Address;
						if (currGatewayAddress.lpSockaddr != NULL)
						{
							if (currGatewayAddress.lpSockaddr->sa_family == AF_INET)
							{
								//Allocate buffer space to hold the IPs.
								myGatewayAddresses[currAdapterIndex][myNumGatewayAddresses[currAdapterIndex]] = (char*)calloc(INET_ADDRSTRLEN, sizeof(char));

								//Get the String representation of the IPV4 address.
								sockaddressIn = (sockaddr_in*)currGatewayAddress.lpSockaddr;
								inet_ntop(AF_INET, &(sockaddressIn->sin_addr), myGatewayAddresses[currAdapterIndex][myNumGatewayAddresses[currAdapterIndex]], INET_ADDRSTRLEN);
								myNumGatewayAddresses[currAdapterIndex]++;
							}
							else if (currGatewayAddress.lpSockaddr->sa_family == AF_INET6)
							{
								//Allocate buffer space to hold the IPs.
								myGatewayAddresses[currAdapterIndex][myNumGatewayAddresses[currAdapterIndex]] = (char*)calloc(INET6_ADDRSTRLEN, sizeof(char));

								//Get the String representation of the IPV6 address.
								sockaddressIn6 = (sockaddr_in6*)currGatewayAddress.lpSockaddr;
								inet_ntop(AF_INET6, &(sockaddressIn6->sin6_addr), myGatewayAddresses[currAdapterIndex][myNumGatewayAddresses[currAdapterIndex]], INET6_ADDRSTRLEN);
								myNumGatewayAddresses[currAdapterIndex]++;
							}
							else
							{
								//Nether IPV4 or IPV6.
								myGatewayAddresses[currAdapterIndex][myNumGatewayAddresses[currAdapterIndex]] = NULL;
							}
						}

						pGatewayAddresses = pGatewayAddresses->Next;
					}

					//Check if nothing was found.
					if (myNumGatewayAddresses[currAdapterIndex] <= 0)
					{
						free(myGatewayAddresses[currAdapterIndex]);
						myGatewayAddresses[currAdapterIndex] = NULL;
					}
				}
				else
				{
					myNumGatewayAddresses[currAdapterIndex] = 0;
					myGatewayAddresses[currAdapterIndex] = NULL;
				}

				//LUID
				myLUIDs[currAdapterIndex] = pAdapterAddresses->Luid.Value;

				//LUID IFType
				const char* luidIfTypeStr = GetIfTypeString(pAdapterAddresses->Luid.Info.IfType);
				size_t luidIfTypeStrLen = strlen(luidIfTypeStr) + 1;
				myLUIDIfTypes[currAdapterIndex] = (char*)calloc(luidIfTypeStrLen, sizeof(char));
				if (strcpy_s(myLUIDIfTypes[currAdapterIndex], luidIfTypeStrLen * sizeof(char), luidIfTypeStr) != 0)
				{
					printf("Error: Failed to get LUID IfType[%u]", currAdapterIndex);
					free(myLUIDIfTypes[currAdapterIndex]);
					myLUIDIfTypes[currAdapterIndex] = NULL;
				}

				//LUID Net Index
				myLUIDNetIndexes[currAdapterIndex] = pAdapterAddresses->Luid.Info.NetLuidIndex;

				//MTU
				myMTUs[currAdapterIndex] = pAdapterAddresses->Mtu;

				//Multicast Addresses
				myNumMulticastAddresses[currAdapterIndex] = 0;
				pMulticastAddresses = pAdapterAddresses->FirstMulticastAddress;
				if (pMulticastAddresses != NULL)
				{
					//Allocate array for each adapter's prefixes (up to 256 prefixes).
					myMulticastAddresses[currAdapterIndex] = (char**)malloc(NetworkInfo::MAX_SUB_ARRAY_SIZE * sizeof(char*));

					while (pMulticastAddresses != NULL)
					{
						SOCKET_ADDRESS currMulticastAddress = pMulticastAddresses->Address;
						if (currMulticastAddress.lpSockaddr != NULL)
						{
							if (currMulticastAddress.lpSockaddr->sa_family == AF_INET)
							{
								//Allocate buffer space to hold the IPs.
								myMulticastAddresses[currAdapterIndex][myNumMulticastAddresses[currAdapterIndex]] = (char*)calloc(INET_ADDRSTRLEN, sizeof(char));

								//Get the String representation of the IPV4 address.
								sockaddressIn = (sockaddr_in*)currMulticastAddress.lpSockaddr;
								inet_ntop(AF_INET, &(sockaddressIn->sin_addr), myMulticastAddresses[currAdapterIndex][myNumMulticastAddresses[currAdapterIndex]], INET_ADDRSTRLEN);
								myNumMulticastAddresses[currAdapterIndex]++;
							}
							else if (currMulticastAddress.lpSockaddr->sa_family == AF_INET6)
							{
								//Allocate buffer space to hold the IPs.
								myMulticastAddresses[currAdapterIndex][myNumMulticastAddresses[currAdapterIndex]] = (char*)calloc(INET6_ADDRSTRLEN, sizeof(char));

								//Get the String representation of the IPV6 address.
								sockaddressIn6 = (sockaddr_in6*)currMulticastAddress.lpSockaddr;
								inet_ntop(AF_INET6, &(sockaddressIn6->sin6_addr), myMulticastAddresses[currAdapterIndex][myNumMulticastAddresses[currAdapterIndex]], INET6_ADDRSTRLEN);
								myNumMulticastAddresses[currAdapterIndex]++;
							}
							else
							{
								//Nether IPV4 or IPV6.
								myMulticastAddresses[currAdapterIndex][myNumMulticastAddresses[currAdapterIndex]] = NULL;
							}
						}

						pMulticastAddresses = pMulticastAddresses->Next;
					}

					//Check if nothing was found.
					if (myNumMulticastAddresses[currAdapterIndex] <= 0)
					{
						free(myMulticastAddresses[currAdapterIndex]);
						myMulticastAddresses[currAdapterIndex] = NULL;
					}
				}
				else
				{
					myNumMulticastAddresses[currAdapterIndex] = 0;
					myMulticastAddresses[currAdapterIndex] = NULL;
				}

				//Network GUID
				myNetworkGUIDs[currAdapterIndex] = (char*)calloc(NetworkInfo::MAX_SUB_ARRAY_SIZE, sizeof(char));
				if (sprintf_s(myNetworkGUIDs[currAdapterIndex], NetworkInfo::MAX_SUB_ARRAY_SIZE * sizeof(char), "%lu %hd %hd %s", pAdapterAddresses->NetworkGuid.Data1, pAdapterAddresses->NetworkGuid.Data2, pAdapterAddresses->NetworkGuid.Data3, pAdapterAddresses->NetworkGuid.Data4) == -1)
				{
					printf("Error: Failed to get Network GUID components[%u]", currAdapterIndex);
					free(myNetworkGUIDs[currAdapterIndex]);
					myNetworkGUIDs[currAdapterIndex] = NULL;
				}

				//OPER Status
				IF_OPER_STATUS currAdapterOperStatus = pAdapterAddresses->OperStatus;
				const char* operStatusStr;
				if (currAdapterOperStatus == IF_OPER_STATUS::IfOperStatusUp)
				{
					operStatusStr = "IfOperStatusUp";
				}
				else if (currAdapterOperStatus == IF_OPER_STATUS::IfOperStatusDown)
				{
					operStatusStr = "IfOperStatusDown";
				}
				else if (currAdapterOperStatus == IF_OPER_STATUS::IfOperStatusTesting)
				{
					operStatusStr = "IfOperStatusTesting";
				}
				else if (currAdapterOperStatus == IF_OPER_STATUS::IfOperStatusUnknown)
				{
					operStatusStr = "IfOperStatusUnknown";
				}
				else if (currAdapterOperStatus == IF_OPER_STATUS::IfOperStatusDormant)
				{
					operStatusStr = "IfOperStatusDormant";
				}
				else if (currAdapterOperStatus == IF_OPER_STATUS::IfOperStatusNotPresent)
				{
					operStatusStr = "IfOperStatusNotPresent";
				}
				else // if (currAdapterOperStatus == IF_OPER_STATUS::IfOperStatusLowerLayerDown)
				{
					operStatusStr = "IfOperStatusLowerLayerDown";
				}
				size_t operStatusStrLen = strlen(operStatusStr) + 1;
				myOperStatuses[currAdapterIndex] = (char*)calloc(operStatusStrLen, sizeof(char));
				if (strcpy_s(myOperStatuses[currAdapterIndex], operStatusStrLen, operStatusStr) != 0)
				{
					printf("Error: Failed to get OPER Status[%u]", currAdapterIndex);
					free(myOperStatuses[currAdapterIndex]);
					myOperStatuses[currAdapterIndex] = NULL;
				}

				//Physical Address
				myPhysicalAddressLengths[currAdapterIndex] = pAdapterAddresses->PhysicalAddressLength;
				if (myPhysicalAddressLengths[currAdapterIndex] > 0)
				{
					myPhysicalAddresses[currAdapterIndex] = (char*)calloc(MAX_ADAPTER_ADDRESS_LENGTH, sizeof(char*));
					if (sprintf_s(myPhysicalAddresses[currAdapterIndex], MAX_ADAPTER_ADDRESS_LENGTH, "%x\n", pAdapterAddresses->PhysicalAddress) == -1)
					{
						printf("Error: Failed to get Physical Address[%u]", currAdapterIndex);
						free(myPhysicalAddresses[currAdapterIndex]);
						myPhysicalAddresses[currAdapterIndex] = NULL;
					}
				}
				else
				{
					myPhysicalAddresses[currAdapterIndex] = NULL;
				}

				//Receive Link Speed
				myReceiveLinkSpeeds[currAdapterIndex] = pAdapterAddresses->ReceiveLinkSpeed;

				//Transmit Link Speed
				myTransmitLinkSpeeds[currAdapterIndex] = pAdapterAddresses->TransmitLinkSpeed;

				//Tunnel Type
				TUNNEL_TYPE currAdapterTunnelType = pAdapterAddresses->TunnelType;
				const char* tunnelTypeStr;
				if (currAdapterTunnelType == TUNNEL_TYPE::TUNNEL_TYPE_NONE)
				{
					tunnelTypeStr = "TUNNEL_TYPE_NONE";
				}
				else if (currAdapterTunnelType == TUNNEL_TYPE::TUNNEL_TYPE_OTHER)
				{
					tunnelTypeStr = "TUNNEL_TYPE_OTHER";
				}
				else if (currAdapterTunnelType == TUNNEL_TYPE::TUNNEL_TYPE_DIRECT)
				{
					tunnelTypeStr = "TUNNEL_TYPE_DIRECT";
				}
				else if (currAdapterTunnelType == TUNNEL_TYPE::TUNNEL_TYPE_6TO4)
				{
					tunnelTypeStr = "TUNNEL_TYPE_6TO4";
				}
				else if (currAdapterTunnelType == TUNNEL_TYPE::TUNNEL_TYPE_ISATAP)
				{
					tunnelTypeStr = "TUNNEL_TYPE_ISATAP";
				}
				else if (currAdapterTunnelType == TUNNEL_TYPE::TUNNEL_TYPE_TEREDO)
				{
					tunnelTypeStr = "TUNNEL_TYPE_TEREDO";
				}
				else // if (currAdapterTunnelType == TUNNEL_TYPE::TUNNEL_TYPE_IPHTTPS)
				{
					tunnelTypeStr = "TUNNEL_TYPE_IPHTTPS";
				}
				size_t tunnelTypeStrLen = strlen(tunnelTypeStr) + 1;
				myTunnelTypes[currAdapterIndex] = (char*)calloc(tunnelTypeStrLen, sizeof(char));
				if (strcpy_s(myTunnelTypes[currAdapterIndex], tunnelTypeStrLen, tunnelTypeStr) != 0)
				{
					printf("Error: Failed to get Tunnel Type[%u]", currAdapterIndex);
					free(myTunnelTypes[currAdapterIndex]);
					myTunnelTypes[currAdapterIndex] = NULL;
				}

				//Unicast Addresses.
				myNumUnicastAddresses[currAdapterIndex] = 0;
				pUnicastAddresses = pAdapterAddresses->FirstUnicastAddress;
				if (pUnicastAddresses != NULL)
				{
					//Allocate array for each adapter's prefixes (up to 256 prefixes).
					myUnicastAddresses[currAdapterIndex] = (char**)malloc(NetworkInfo::MAX_SUB_ARRAY_SIZE * sizeof(char*));

					while (pUnicastAddresses != NULL)
					{
						SOCKET_ADDRESS currUnicastAddress = pUnicastAddresses->Address;
						if (currUnicastAddress.lpSockaddr != NULL)
						{
							if (currUnicastAddress.lpSockaddr->sa_family == AF_INET)
							{
								//Allocate buffer space to hold the IPs.
								myUnicastAddresses[currAdapterIndex][myNumUnicastAddresses[currAdapterIndex]] = (char*)calloc(INET_ADDRSTRLEN, sizeof(char));

								//Get the String representation of the IPV4 address.
								sockaddressIn = (sockaddr_in*)currUnicastAddress.lpSockaddr;
								inet_ntop(AF_INET, &(sockaddressIn->sin_addr), myUnicastAddresses[currAdapterIndex][myNumUnicastAddresses[currAdapterIndex]], INET_ADDRSTRLEN);
								myNumUnicastAddresses[currAdapterIndex]++;
							}
							else if (currUnicastAddress.lpSockaddr->sa_family == AF_INET6)
							{
								//Allocate buffer space to hold the IPs.
								myUnicastAddresses[currAdapterIndex][myNumUnicastAddresses[currAdapterIndex]] = (char*)calloc(INET6_ADDRSTRLEN, sizeof(char));

								//Get the String representation of the IPV6 address.
								sockaddressIn6 = (sockaddr_in6*)currUnicastAddress.lpSockaddr;
								inet_ntop(AF_INET6, &(sockaddressIn6->sin6_addr), myUnicastAddresses[currAdapterIndex][myNumUnicastAddresses[currAdapterIndex]], INET6_ADDRSTRLEN);
								myNumUnicastAddresses[currAdapterIndex]++;
							}
							else
							{
								//Nether IPV4 or IPV6.
								myUnicastAddresses[currAdapterIndex][myNumUnicastAddresses[currAdapterIndex]] = NULL;
							}
						}

						pUnicastAddresses = pUnicastAddresses->Next;
					}

					//Check if nothing was found.
					if (myNumUnicastAddresses[currAdapterIndex] <= 0)
					{
						free(myUnicastAddresses[currAdapterIndex]);
						myUnicastAddresses[currAdapterIndex] = NULL;
					}
				}
				else
				{
					myNumUnicastAddresses[currAdapterIndex] = 0;
					myUnicastAddresses[currAdapterIndex] = NULL;
				}

				//Windows Server Address
				myNumWindowsServerAddresses[currAdapterIndex] = 0;
				pWindowsServerAddresses = pAdapterAddresses->FirstWinsServerAddress;
				if (pWindowsServerAddresses != NULL)
				{
					//Allocate array for each adapter's prefixes (up to 256 prefixes).
					myWindowsServerAddresses[currAdapterIndex] = (char**)malloc(NetworkInfo::MAX_SUB_ARRAY_SIZE * sizeof(char*));

					while (pWindowsServerAddresses != NULL)
					{
						SOCKET_ADDRESS currWindowsServerAddress = pWindowsServerAddresses->Address;
						if (currWindowsServerAddress.lpSockaddr != NULL)
						{
							if (currWindowsServerAddress.lpSockaddr->sa_family == AF_INET)
							{
								//Allocate buffer space to hold the IPs.
								myWindowsServerAddresses[currAdapterIndex][myNumWindowsServerAddresses[currAdapterIndex]] = (char*)calloc(INET_ADDRSTRLEN, sizeof(char));

								//Get the String representation of the IPV4 address.
								sockaddressIn = (sockaddr_in*)currWindowsServerAddress.lpSockaddr;
								inet_ntop(AF_INET, &(sockaddressIn->sin_addr), myWindowsServerAddresses[currAdapterIndex][myNumWindowsServerAddresses[currAdapterIndex]], INET_ADDRSTRLEN);
								myNumWindowsServerAddresses[currAdapterIndex]++;
							}
							else if (currWindowsServerAddress.lpSockaddr->sa_family == AF_INET6)
							{
								//Allocate buffer space to hold the IPs.
								myWindowsServerAddresses[currAdapterIndex][myNumWindowsServerAddresses[currAdapterIndex]] = (char*)calloc(INET6_ADDRSTRLEN, sizeof(char));

								//Get the String representation of the IPV6 address.
								sockaddressIn6 = (sockaddr_in6*)currWindowsServerAddress.lpSockaddr;
								inet_ntop(AF_INET6, &(sockaddressIn6->sin6_addr), myWindowsServerAddresses[currAdapterIndex][myNumWindowsServerAddresses[currAdapterIndex]], INET6_ADDRSTRLEN);
								myNumWindowsServerAddresses[currAdapterIndex]++;
							}
							else
							{
								//Nether IPV4 or IPV6.
								myWindowsServerAddresses[currAdapterIndex][myNumWindowsServerAddresses[currAdapterIndex]] = NULL;
							}
						}

						pWindowsServerAddresses = pWindowsServerAddresses->Next;
					}

					//Check if nothing was found.
					if (myNumWindowsServerAddresses[currAdapterIndex] <= 0)
					{
						free(myWindowsServerAddresses[currAdapterIndex]);
						myWindowsServerAddresses[currAdapterIndex] = NULL;
					}
				}
				else
				{
					myNumWindowsServerAddresses[currAdapterIndex] = 0;
					myWindowsServerAddresses[currAdapterIndex] = NULL;
				}

				//Zone Indexes
				myZoneIndexes[currAdapterIndex] = (unsigned long*)calloc(16, sizeof(unsigned long));
				for (unsigned int currZoneIndex = 0; currZoneIndex < 16; currZoneIndex++)
				{
					myZoneIndexes[currAdapterIndex][currZoneIndex] = pAdapterAddresses->ZoneIndices[currZoneIndex];
				}

				pAdapterAddresses = pAdapterAddresses->Next;
			}

			if (headAdapterAddress != NULL)
			{
				delete headAdapterAddress;
				headAdapterAddress = NULL;
			}
		}
	}
#endif
}

bool WindowsNetworkInfo::InitializeBuffers(unsigned int numAdapters)
{
	if (numAdapters > 0)
	{
		myNumAdapterPrefixAddresses =	(unsigned int*)malloc(numAdapters * sizeof(unsigned int));
		myNumAnycastAddresses =			(unsigned int*)malloc(numAdapters * sizeof(unsigned int));
		myNumDNSServerAddresses =		(unsigned int*)malloc(numAdapters * sizeof(unsigned int));
		myNumDNSSuffixes =				(unsigned int*)malloc(numAdapters * sizeof(unsigned int));
		myNumGatewayAddresses =			(unsigned int*)malloc(numAdapters * sizeof(unsigned int));
		myNumMulticastAddresses =		(unsigned int*)malloc(numAdapters * sizeof(unsigned int));
		myPhysicalAddressLengths =		(unsigned int*)malloc(numAdapters * sizeof(unsigned int));
		myNumUnicastAddresses =			(unsigned int*)malloc(numAdapters * sizeof(unsigned int));
		myNumWindowsServerAddresses =	(unsigned int*)malloc(numAdapters * sizeof(unsigned int));

		//Allocate data arrays now that we know how many adapters there are.
		myAdapterNames =			(char**)malloc(numAdapters * sizeof(char*));
		myAdapterPrefixAddresses =	(char***)malloc(numAdapters * sizeof(char**));
		myAnycastAddresses =		(char***)malloc(numAdapters * sizeof(char**));
		myConnectionTypes =			(char**)malloc(numAdapters * sizeof(char*));
		myCompartmentTypes =		(char**)malloc(numAdapters * sizeof(char*));
		myDescriptions =			(wchar_t**)malloc(numAdapters * sizeof(wchar_t*));
		myDHCPV4Servers =			(char**)malloc(numAdapters * sizeof(char*));
		myDHCPV6Servers =			(char**)malloc(numAdapters * sizeof(char*));
		myDHCPV6ClientDUIDs =		(char**)malloc(numAdapters * sizeof(char*));
		myDHCPV6Iads =				(unsigned long*)malloc(numAdapters * sizeof(unsigned long));
		myDNSServerAddresses =		(char***)malloc(numAdapters * sizeof(char**));
		myDNSSuffixes =				(wchar_t***)malloc((numAdapters + 1) * sizeof(wchar_t**));
		myFlags =					(char**)malloc(numAdapters * sizeof(char*));
		myFriendlyNames =			(wchar_t**)malloc(numAdapters * sizeof(wchar_t*));
		myIfTypes =					(char**)malloc(numAdapters * sizeof(char*));
		myIPV4IfIndexes =			(unsigned long*)malloc(numAdapters * sizeof(unsigned long));
		myIPV6IfIndexes =			(unsigned long*)malloc(numAdapters * sizeof(unsigned long));
		myIPV4Metrics =				(unsigned long*)malloc(numAdapters * sizeof(unsigned long));
		myIPV6Metrics =				(unsigned long*)malloc(numAdapters * sizeof(unsigned long));
		myGatewayAddresses =		(char***)malloc(numAdapters * sizeof(char**));
		myLUIDs =					(unsigned __int64*)malloc(numAdapters * sizeof(unsigned __int64));
		myLUIDIfTypes =				(char**)malloc(numAdapters * sizeof(char*));
		myLUIDNetIndexes =			(unsigned __int64*)malloc(numAdapters * sizeof(unsigned __int64));
		myMTUs =					(unsigned long*)malloc(numAdapters * sizeof(unsigned long));
		myMulticastAddresses =		(char***)malloc(numAdapters * sizeof(char**));
		myNetworkGUIDs =			(char**)malloc(numAdapters * sizeof(char*));
		myOperStatuses =			(char**)malloc(numAdapters * sizeof(char*));
		myPhysicalAddresses =		(char**)malloc(numAdapters * sizeof(char*));
		myReceiveLinkSpeeds =		(unsigned __int64*)malloc(numAdapters * sizeof(unsigned __int64));
		myTransmitLinkSpeeds =		(unsigned __int64*)malloc(numAdapters * sizeof(unsigned __int64));
		myTunnelTypes =				(char**)malloc(numAdapters * sizeof(char*));
		myUnicastAddresses =		(char***)malloc(numAdapters * sizeof(char**));
		myWindowsServerAddresses =	(char***)malloc(numAdapters * sizeof(char**));
		myZoneIndexes =				(unsigned long**)malloc(numAdapters * sizeof(unsigned long*));

		return true;
	}

	return false;
}

void WindowsNetworkInfo::DestroyBuffers(unsigned int numAdapters)
{
	for (unsigned int currAdapterIndex = 0; currAdapterIndex < numAdapters; currAdapterIndex++)
	{
		if (myAdapterNames[currAdapterIndex] != NULL)
		{
			delete myAdapterNames[currAdapterIndex];
			myAdapterNames[currAdapterIndex] = NULL;
		}

		if (myNumAdapterPrefixAddresses[currAdapterIndex] > 0)
		{
			for (unsigned int currAdapterPrefixIndex = 0; currAdapterPrefixIndex < myNumAdapterPrefixAddresses[currAdapterIndex]; currAdapterPrefixIndex++)
			{
				if (myAdapterPrefixAddresses[currAdapterIndex][currAdapterPrefixIndex] != NULL)
				{
					delete myAdapterPrefixAddresses[currAdapterIndex][currAdapterPrefixIndex];
					myAdapterPrefixAddresses[currAdapterIndex][currAdapterPrefixIndex] = NULL;
				}
			}

			delete myAdapterPrefixAddresses[currAdapterIndex];
			myAdapterPrefixAddresses[currAdapterIndex] = NULL;
			myNumAdapterPrefixAddresses[currAdapterIndex] = 0;
		}

		if (myNumAnycastAddresses[currAdapterIndex] > 0)
		{
			for (unsigned int currAnycastAddressIndex = 0; currAnycastAddressIndex < myNumAnycastAddresses[currAdapterIndex]; currAnycastAddressIndex++)
			{
				if (myAnycastAddresses[currAdapterIndex][currAnycastAddressIndex] != NULL)
				{
					delete myAnycastAddresses[currAdapterIndex][currAnycastAddressIndex];
					myAnycastAddresses[currAdapterIndex][currAnycastAddressIndex] = NULL;
				}
			}

			delete myAnycastAddresses[currAdapterIndex];
			myAnycastAddresses[currAdapterIndex] = NULL;
			myNumAnycastAddresses[currAdapterIndex] = 0;
		}

		if (myConnectionTypes[currAdapterIndex] != NULL)
		{
			delete myConnectionTypes[currAdapterIndex];
			myConnectionTypes[currAdapterIndex] = NULL;
		}

		if (myCompartmentTypes[currAdapterIndex] != NULL)
		{
			delete myCompartmentTypes[currAdapterIndex];
			myCompartmentTypes[currAdapterIndex] = NULL;
		}

		if (myDescriptions[currAdapterIndex] != NULL)
		{
			delete myDescriptions[currAdapterIndex];
			myDescriptions[currAdapterIndex] = NULL;
		}

		if (myDHCPV4Servers[currAdapterIndex] != NULL)
		{
			delete myDHCPV4Servers[currAdapterIndex];
			myDHCPV4Servers[currAdapterIndex] = NULL;
		}

		if (myDHCPV6Servers[currAdapterIndex] != NULL)
		{
			delete myDHCPV6Servers[currAdapterIndex];
			myDHCPV6Servers[currAdapterIndex] = NULL;
		}

		if (myDHCPV6ClientDUIDs[currAdapterIndex] != NULL)
		{
			delete myDHCPV6ClientDUIDs[currAdapterIndex];
			myDHCPV6ClientDUIDs[currAdapterIndex] = NULL;
		}

		if (myDHCPV6Iads != NULL)
		{
			myDHCPV6Iads[currAdapterIndex] = 0;
		}

		if (myNumDNSServerAddresses[currAdapterIndex] > 0)
		{
			for (unsigned int currDNSServerAddressIndex = 0; currDNSServerAddressIndex < myNumDNSServerAddresses[currAdapterIndex]; currDNSServerAddressIndex++)
			{
				delete myDNSServerAddresses[currAdapterIndex][currDNSServerAddressIndex];
				myDNSServerAddresses[currAdapterIndex][currDNSServerAddressIndex] = NULL;
			}

			delete myDNSServerAddresses[currAdapterIndex];
			myDNSServerAddresses[currAdapterIndex] = NULL;
			myNumDNSServerAddresses[currAdapterIndex] = 0;
		}

		if (myNumDNSSuffixes[currAdapterIndex] > 0)
		{
			for (unsigned int currDNSSuffixIndex = 0; currDNSSuffixIndex < myNumDNSSuffixes[currAdapterIndex]; currDNSSuffixIndex++)
			{
				delete myDNSSuffixes[currAdapterIndex][currDNSSuffixIndex];
				myDNSSuffixes[currAdapterIndex][currDNSSuffixIndex] = NULL;
			}

			delete myDNSSuffixes[currAdapterIndex];
			myDNSSuffixes[currAdapterIndex] = NULL;
			myNumDNSSuffixes[currAdapterIndex] = 0;
		}

		if (myFlags[currAdapterIndex] != NULL)
		{
			delete myFlags[currAdapterIndex];
			myFlags[currAdapterIndex] = NULL;
		}

		if (myFriendlyNames[currAdapterIndex] != NULL)
		{
			delete myFriendlyNames[currAdapterIndex];
			myFriendlyNames[currAdapterIndex] = NULL;
		}

		if (myIfTypes[currAdapterIndex] != NULL)
		{
			delete myIfTypes[currAdapterIndex];
			myIfTypes[currAdapterIndex] = NULL;
		}

		if (myIPV4IfIndexes != NULL)
		{
			myIPV4IfIndexes[currAdapterIndex] = 0;
		}

		if (myIPV6IfIndexes != NULL)
		{
			myIPV6IfIndexes[currAdapterIndex] = 0;
		}

		if (myIPV4Metrics != NULL)
		{
			myIPV4Metrics[currAdapterIndex] = 0;
		}

		if (myIPV6Metrics != NULL)
		{
			myIPV6Metrics[currAdapterIndex] = 0;
		}

		if (myNumGatewayAddresses[currAdapterIndex] > 0)
		{
			for (unsigned int currGatewayAddressIndex = 0; currGatewayAddressIndex < myNumGatewayAddresses[currAdapterIndex]; currGatewayAddressIndex++)
			{
				delete myGatewayAddresses[currAdapterIndex][currGatewayAddressIndex];
				myGatewayAddresses[currAdapterIndex][currGatewayAddressIndex] = NULL;
			}

			delete myGatewayAddresses[currAdapterIndex];
			myGatewayAddresses[currAdapterIndex] = NULL;
			myNumGatewayAddresses[currAdapterIndex] = 0;
		}

		if (myLUIDs != NULL)
		{
			myLUIDs[currAdapterIndex] = 0;
		}

		if (myLUIDIfTypes[currAdapterIndex] != NULL)
		{
			delete myLUIDIfTypes[currAdapterIndex];
			myLUIDIfTypes[currAdapterIndex] = NULL;
		}

		if (myLUIDNetIndexes != NULL)
		{
			myLUIDNetIndexes[currAdapterIndex] = 0;
		}

		if (myNumMulticastAddresses[currAdapterIndex] > 0)
		{
			for (unsigned int currMulticastAddressIndex = 0; currMulticastAddressIndex < myNumMulticastAddresses[currAdapterIndex]; currMulticastAddressIndex++)
			{
				delete myMulticastAddresses[currAdapterIndex][currMulticastAddressIndex];
				myMulticastAddresses[currAdapterIndex][currMulticastAddressIndex] = NULL;
			}

			delete myMulticastAddresses[currAdapterIndex];
			myMulticastAddresses[currAdapterIndex] = NULL;
			myNumMulticastAddresses[currAdapterIndex] = 0;
		}

		if (myNetworkGUIDs[currAdapterIndex] != NULL)
		{
			delete myNetworkGUIDs[currAdapterIndex];
			myNetworkGUIDs[currAdapterIndex] = NULL;
		}

		if (myOperStatuses[currAdapterIndex] != NULL)
		{
			delete myOperStatuses[currAdapterIndex];
			myOperStatuses[currAdapterIndex] = NULL;
		}

		if (myPhysicalAddresses[currAdapterIndex] != NULL)
		{
			delete myPhysicalAddresses[currAdapterIndex];
			myPhysicalAddresses[currAdapterIndex] = NULL;
			myPhysicalAddressLengths[currAdapterIndex] = 0;
		}

		if (myReceiveLinkSpeeds != NULL)
		{
			myReceiveLinkSpeeds[currAdapterIndex] = 0;
		}

		if (myTransmitLinkSpeeds != NULL)
		{
			myTransmitLinkSpeeds[currAdapterIndex] = 0;
		}

		if (myTunnelTypes[currAdapterIndex] != NULL)
		{
			delete myTunnelTypes[currAdapterIndex];
			myTunnelTypes[currAdapterIndex] = NULL;
		}

		if (myNumUnicastAddresses[currAdapterIndex] > 0)
		{
			for (unsigned int currUnicastAddressIndex = 0; currUnicastAddressIndex < myNumUnicastAddresses[currAdapterIndex]; currUnicastAddressIndex++)
			{
				delete myUnicastAddresses[currAdapterIndex][currUnicastAddressIndex];
				myUnicastAddresses[currAdapterIndex][currUnicastAddressIndex] = NULL;
			}

			delete myUnicastAddresses[currAdapterIndex];
			myUnicastAddresses[currAdapterIndex] = NULL;
			myNumUnicastAddresses[currAdapterIndex] = 0;
		}

		if (myNumWindowsServerAddresses[currAdapterIndex] > 0)
		{
			for (unsigned int currWinServerAddressIndex = 0; currWinServerAddressIndex < myNumWindowsServerAddresses[currAdapterIndex]; currWinServerAddressIndex++)
			{
				delete myWindowsServerAddresses[currAdapterIndex][currWinServerAddressIndex];
				myWindowsServerAddresses[currAdapterIndex][currWinServerAddressIndex] = NULL;
			}

			delete myWindowsServerAddresses[currAdapterIndex];
			myWindowsServerAddresses[currAdapterIndex] = NULL;
			myNumWindowsServerAddresses[currAdapterIndex] = 0;
		}

		if (myZoneIndexes[currAdapterIndex] != NULL)
		{
			delete myZoneIndexes[currAdapterIndex];
			myZoneIndexes[currAdapterIndex] = NULL;
		}
	}

	//All internal data is deleted and zeroed out, now delete the main data buffers.
	delete myNumAdapterPrefixAddresses;
	myNumAdapterPrefixAddresses = NULL;

	delete myNumAnycastAddresses;
	myNumAnycastAddresses = NULL;

	delete myNumDNSServerAddresses;
	myNumDNSServerAddresses = NULL;

	delete myNumDNSSuffixes;
	myNumDNSSuffixes = NULL;

	delete myNumGatewayAddresses;
	myNumGatewayAddresses = NULL;

	delete myNumMulticastAddresses;
	myNumMulticastAddresses = NULL;

	delete myPhysicalAddressLengths;
	myPhysicalAddressLengths = NULL;

	delete myNumUnicastAddresses;
	myNumUnicastAddresses = NULL;

	delete myNumWindowsServerAddresses;
	myNumWindowsServerAddresses = NULL;

	delete myAdapterNames;
	myAdapterNames = NULL;

	delete myAdapterPrefixAddresses;
	myAdapterPrefixAddresses = NULL;

	delete myAnycastAddresses;
	myAnycastAddresses = NULL;

	delete myConnectionTypes;
	myConnectionTypes = NULL;

	delete myCompartmentTypes;
	myCompartmentTypes = NULL;

	delete myDescriptions;
	myDescriptions = NULL;

	delete myDHCPV4Servers;
	myDHCPV4Servers = NULL;

	delete myDHCPV6Servers;
	myDHCPV6Servers = NULL;

	delete myDHCPV6ClientDUIDs;
	myDHCPV6ClientDUIDs = NULL;

	delete myDHCPV6Iads;
	myDHCPV6Iads = NULL;

	delete myDNSServerAddresses;
	myDNSServerAddresses = NULL;

	delete myDNSSuffixes;
	myDNSSuffixes = NULL;

	delete myFlags;
	myFlags = NULL;

	delete myFriendlyNames;
	myFriendlyNames = NULL;

	delete myIfTypes;
	myIfTypes = NULL;

	delete myIPV4IfIndexes;
	myIPV4IfIndexes = NULL;

	delete myIPV6IfIndexes;
	myIPV6IfIndexes = NULL;

	delete myIPV4Metrics;
	myIPV4Metrics = NULL;

	delete myIPV6Metrics;
	myIPV6Metrics = NULL;

	delete myGatewayAddresses;
	myGatewayAddresses = NULL;

	delete myLUIDs;
	myLUIDs = NULL;

	delete myLUIDIfTypes;
	myLUIDIfTypes = NULL;

	delete myLUIDNetIndexes;
	myLUIDNetIndexes = NULL;

	delete myMTUs;
	myMTUs = NULL;

	delete myMulticastAddresses;
	myMulticastAddresses = NULL;

	delete myNetworkGUIDs;
	myNetworkGUIDs = NULL;

	delete myOperStatuses;
	myOperStatuses = NULL;

	delete myPhysicalAddresses;
	myPhysicalAddresses = NULL;

	delete myReceiveLinkSpeeds;
	myReceiveLinkSpeeds = NULL;

	delete myTransmitLinkSpeeds;
	myTransmitLinkSpeeds = NULL;

	delete myTunnelTypes;
	myTunnelTypes = NULL;

	delete myUnicastAddresses;
	myUnicastAddresses = NULL;

	delete myWindowsServerAddresses;
	myWindowsServerAddresses = NULL;

	delete myZoneIndexes;
	myZoneIndexes = NULL;

}

char* WindowsNetworkInfo::GetInfo()
{
	size_t retValLen = 2 << 15;
	char* retVal = (char*)calloc(retValLen, sizeof(char));

	sprintf_s(retVal, retValLen, "Number of Adapters:%u\n", myNumAdapters);
	 
	for (unsigned int currAdapterIndex = 0; currAdapterIndex < myNumAdapters; currAdapterIndex++)
	{
		sprintf_s(retVal, retValLen, "%s==========================\n", retVal);

		if (myAdapterNames[currAdapterIndex] != NULL)
		{
			sprintf_s(retVal, retValLen, "%sAdapterName[%u]:%s\n", retVal, currAdapterIndex, myAdapterNames[currAdapterIndex]);
		}
		
		if (myNumAdapterPrefixAddresses[currAdapterIndex] > 0)
		{
			sprintf_s(retVal, retValLen, "%sNumber of Adapter Prefix Addresses[%u]:%u\n", retVal, currAdapterIndex, myNumAdapterPrefixAddresses[currAdapterIndex]);
			for (unsigned int currAdapterPrefixIndex = 0; currAdapterPrefixIndex < myNumAdapterPrefixAddresses[currAdapterIndex]; currAdapterPrefixIndex++)
			{
				sprintf_s(retVal, retValLen, "%sAdapter Prefix Address[%u][%u]:%s\n", retVal, currAdapterIndex, currAdapterPrefixIndex, myAdapterPrefixAddresses[currAdapterIndex][currAdapterPrefixIndex]);
			}
		}

		if (myNumAnycastAddresses[currAdapterIndex] > 0)
		{
			sprintf_s(retVal, retValLen, "%sNumber of Anycast Addresses[%u]:%u\n", retVal, currAdapterIndex, myNumAnycastAddresses[currAdapterIndex]);
			for (unsigned int currAnycastAddressIndex = 0; currAnycastAddressIndex < myNumAnycastAddresses[currAdapterIndex]; currAnycastAddressIndex++)
			{
				sprintf_s(retVal, retValLen, "%sAdapter Anycast Addresses[%u][%u]:%s\n", retVal, currAdapterIndex, currAnycastAddressIndex, myAnycastAddresses[currAdapterIndex][currAnycastAddressIndex]);
			}
		}

		if (myConnectionTypes[currAdapterIndex] != NULL)
		{
			sprintf_s(retVal, retValLen, "%sConnection Type[%u]:%s\n", retVal, currAdapterIndex, myConnectionTypes[currAdapterIndex]);
		}

		if (myCompartmentTypes[currAdapterIndex] != NULL)
		{
			sprintf_s(retVal, retValLen, "%sCompartment Type[%u]:%s\n", retVal, currAdapterIndex, myCompartmentTypes[currAdapterIndex]);
		}

		if (myDescriptions[currAdapterIndex] != NULL)
		{
			sprintf_s(retVal, retValLen, "%sDescriptions Type[%u]:%wS\n", retVal, currAdapterIndex, myDescriptions[currAdapterIndex]);
		}

		if (myDHCPV4Servers[currAdapterIndex] != NULL)
		{
			sprintf_s(retVal, retValLen, "%sDHCPV4 Servers [%u]:%s\n", retVal, currAdapterIndex, myDHCPV4Servers[currAdapterIndex]);
		}

		if (myDHCPV6Servers[currAdapterIndex] != NULL)
		{
			sprintf_s(retVal, retValLen, "%sDHCPV6 Servers [%u]:%s\n", retVal, currAdapterIndex, myDHCPV6Servers[currAdapterIndex]);
		}

		if (myDHCPV6ClientDUIDs[currAdapterIndex] != NULL)
		{
			sprintf_s(retVal, retValLen, "%sDHCPV6 Client DUID [%u]:%s\n", retVal, currAdapterIndex, myDHCPV6ClientDUIDs[currAdapterIndex]);
		}

		if (myDHCPV6Iads[currAdapterIndex] != NULL)
		{
			sprintf_s(retVal, retValLen, "%sDHCPV6 IADS [%u]:%ul\n", retVal, currAdapterIndex, myDHCPV6Iads[currAdapterIndex]);
		}

		if (myNumDNSServerAddresses[currAdapterIndex] > 0)
		{
			sprintf_s(retVal, retValLen, "%sNumber of DNS Server Addresses[%u]:%u\n", retVal, currAdapterIndex, myNumDNSServerAddresses[currAdapterIndex]);
			for (unsigned int currDNSServerAddressIndex = 0; currDNSServerAddressIndex < myNumDNSServerAddresses[currAdapterIndex]; currDNSServerAddressIndex++)
			{
				sprintf_s(retVal, retValLen, "%sDNS Server Addresses[%u][%u]:%s\n", retVal, currAdapterIndex, currDNSServerAddressIndex, myDNSServerAddresses[currAdapterIndex][currDNSServerAddressIndex]);
			}
		}

		if (myNumDNSSuffixes[currAdapterIndex] > 0)
		{
			sprintf_s(retVal, retValLen, "%sNumber of DNS Suffixes[%u]:%u\n", retVal, currAdapterIndex, myNumDNSSuffixes[currAdapterIndex]);
			for (unsigned int currDNSSuffixIndex = 0; currDNSSuffixIndex < myNumDNSSuffixes[currAdapterIndex]; currDNSSuffixIndex++)
			{
				sprintf_s(retVal, retValLen, "%s\tDNS Suffixes[%u][%u]:%wS\n", retVal, currAdapterIndex, currDNSSuffixIndex, myDNSSuffixes[currAdapterIndex][currDNSSuffixIndex]);
			}
		}

		if (myFlags[currAdapterIndex] != NULL)
		{
			sprintf_s(retVal, retValLen, "%sFlag[%u]:%s\n", retVal, currAdapterIndex, myFlags[currAdapterIndex]);
		}
		
		if (myFriendlyNames[currAdapterIndex] != NULL)
		{
			sprintf_s(retVal, retValLen, "%sFriendly Name[%u]:%wS\n", retVal, currAdapterIndex, myFriendlyNames[currAdapterIndex]);
		}

		if (myIfTypes[currAdapterIndex] != NULL)
		{
			sprintf_s(retVal, retValLen, "%sIf Type[%u]:%s\n", retVal, currAdapterIndex, myIfTypes[currAdapterIndex]);
		}

		if (myIPV4IfIndexes[currAdapterIndex] != NULL)
		{
			sprintf_s(retVal, retValLen, "%sIPV4 If Index[%u]:%ul\n", retVal, currAdapterIndex, myIPV4IfIndexes[currAdapterIndex]);
		}
		
		if (myIPV6IfIndexes[currAdapterIndex] != NULL)
		{
			sprintf_s(retVal, retValLen, "%sIPV6 If Index[%u]:%ul\n", retVal, currAdapterIndex, myIPV6IfIndexes[currAdapterIndex]);
		}

		if (myIPV4Metrics[currAdapterIndex] != NULL)
		{
			sprintf_s(retVal, retValLen, "%sIPV4 Metrics[%u]:%ul\n", retVal, currAdapterIndex, myIPV4Metrics[currAdapterIndex]);
		}

		if (myIPV6Metrics[currAdapterIndex] != NULL)
		{
			sprintf_s(retVal, retValLen, "%sIPV6 Metrics[%u]:%ul\n", retVal, currAdapterIndex, myIPV6Metrics[currAdapterIndex]);
		}

		if (myNumGatewayAddresses[currAdapterIndex] > 0)
		{
			sprintf_s(retVal, retValLen, "%sNumber of Gateway Addresses[%u]:%u\n", retVal, currAdapterIndex, myNumGatewayAddresses[currAdapterIndex]);
			for (unsigned int currGatewayAddressIndex = 0; currGatewayAddressIndex < myNumGatewayAddresses[currAdapterIndex]; currGatewayAddressIndex++)
			{
				sprintf_s(retVal, retValLen, "%sGateway Address[%u][%u]:%s\n", retVal, currAdapterIndex, currGatewayAddressIndex, myGatewayAddresses[currAdapterIndex][currGatewayAddressIndex]);
			}
		}

		if (myLUIDs[currAdapterIndex] > 0)
		{
			sprintf_s(retVal, retValLen, "%sLUID[%u]:%I64u\n", retVal, currAdapterIndex, myLUIDs[currAdapterIndex]);
		}

		if (myLUIDIfTypes[currAdapterIndex] != NULL)
		{
			sprintf_s(retVal, retValLen, "%sLUID If Type[%u]:%s\n", retVal, currAdapterIndex, myLUIDIfTypes[currAdapterIndex]);
		}

		if (myLUIDNetIndexes[currAdapterIndex] > 0)
		{
			sprintf_s(retVal, retValLen, "%sLUID Net Index[%u]:%I64u\n", retVal, currAdapterIndex, myLUIDNetIndexes[currAdapterIndex]);
		}

		if (myMTUs[currAdapterIndex] != NULL)
		{
			sprintf_s(retVal, retValLen, "%sMTU[%u]:%ul\n", retVal, currAdapterIndex, myMTUs[currAdapterIndex]);
		}

		if (myNumMulticastAddresses[currAdapterIndex] > 0)
		{
			sprintf_s(retVal, retValLen, "%sNumber of Multicast Addresses[%u]:%u\n", retVal, currAdapterIndex, myNumMulticastAddresses[currAdapterIndex]);
			for (unsigned int currMulticastAddressIndex = 0; currMulticastAddressIndex < myNumMulticastAddresses[currAdapterIndex]; currMulticastAddressIndex++)
			{
				sprintf_s(retVal, retValLen, "%sMulticast Address[%u][%u]:%s\n", retVal, currAdapterIndex, currMulticastAddressIndex, myMulticastAddresses[currAdapterIndex][currMulticastAddressIndex]);
			}
		}

		if (myNetworkGUIDs[currAdapterIndex] != NULL)
		{
			sprintf_s(retVal, retValLen, "%sLUID If Type[%u]:%s\n", retVal, currAdapterIndex, myNetworkGUIDs[currAdapterIndex]);
		}

		if (myOperStatuses[currAdapterIndex] != NULL)
		{
			sprintf_s(retVal, retValLen, "%sOper Status[%u]:%s\n", retVal, currAdapterIndex, myOperStatuses[currAdapterIndex]);
		}

		if (myPhysicalAddresses[currAdapterIndex] != NULL)
		{
			sprintf_s(retVal, retValLen, "%sPhysical Address[%u]:%s\n", retVal, currAdapterIndex, myPhysicalAddresses[currAdapterIndex]);
		}

		if (myReceiveLinkSpeeds[currAdapterIndex] > 0)
		{
			sprintf_s(retVal, retValLen, "%sReceive Link Speed[%u]:%I64u\n", retVal, currAdapterIndex, myReceiveLinkSpeeds[currAdapterIndex]);
		}

		if (myTransmitLinkSpeeds[currAdapterIndex] > 0)
		{
			sprintf_s(retVal, retValLen, "%sTransmit Link Speed[%u]:%I64u\n", retVal, currAdapterIndex, myTransmitLinkSpeeds[currAdapterIndex]);
		}

		if (myTunnelTypes[currAdapterIndex] != NULL)
		{
			sprintf_s(retVal, retValLen, "%sTunnel Type[%u]:%s\n", retVal, currAdapterIndex, myTunnelTypes[currAdapterIndex]);
		}

		if (myNumUnicastAddresses[currAdapterIndex] > 0)
		{
			sprintf_s(retVal, retValLen, "%s\tNumber of Unicast Addresses[%u]:%u\n", retVal, currAdapterIndex, myNumUnicastAddresses[currAdapterIndex]);
			for (unsigned int currUnicastAddressIndex = 0; currUnicastAddressIndex < myNumUnicastAddresses[currAdapterIndex]; currUnicastAddressIndex++)
			{
				sprintf_s(retVal, retValLen, "%s\tUnicast Address[%u][%u]:%s\n", retVal, currAdapterIndex, currUnicastAddressIndex, myUnicastAddresses[currAdapterIndex][currUnicastAddressIndex]);
			}
		}

		if (myNumWindowsServerAddresses[currAdapterIndex] > 0)
		{
			sprintf_s(retVal, retValLen, "%sNumber of Windows Server Addresses[%u]:%u\n", retVal, currAdapterIndex, myNumWindowsServerAddresses[currAdapterIndex]);
			for (unsigned int currWinServerAddressIndex = 0; currWinServerAddressIndex < myNumWindowsServerAddresses[currAdapterIndex]; currWinServerAddressIndex++)
			{
				sprintf_s(retVal, retValLen, "%sWindows Server Address[%u][%u]:%s\n", retVal, currAdapterIndex, currWinServerAddressIndex, myWindowsServerAddresses[currAdapterIndex][currWinServerAddressIndex]);
			}
		}

		if (myZoneIndexes[currAdapterIndex] > 0)
		{
			for (unsigned int currZoneIndex = 0; currZoneIndex < 16; currZoneIndex++)
			{
				sprintf_s(retVal, retValLen, "%sZone Indexes[%u][%u]:%ul\n", retVal, currAdapterIndex, currZoneIndex, myZoneIndexes[currAdapterIndex][currZoneIndex]);
			}
		}
	}

	return retVal;
}


const char* WindowsNetworkInfo::GetIfTypeString(unsigned long ifType)
{
	if (ifType <= 1)
	{
		return "IF_TYPE_OTHER";
	}
	else if (ifType > 1 && ifType <= 50)
	{
		if (ifType == 2)
		{
			return "IF_TYPE_REGULAR_1822";
		}
		else if (ifType == 3)
		{
			return "IF_TYPE_HDH_1822";
		}
		else if (ifType == 4)
		{
			return "IF_TYPE_DDN_X25";
		}
		else if (ifType == 5)
		{
			return "IF_TYPE_RFC877_X25";
		}
		else if (ifType == 6)
		{
			return "IF_TYPE_ETHERNET_CSMACD";
		}
		else if (ifType == 7)
		{
			return "IF_TYPE_IS088023_CSMACD";
		}
		else if (ifType == 8)
		{
			return "IF_TYPE_ISO88024_TOKENBUS";
		}
		else if (ifType == 9)
		{
			return "IF_TYPE_ISO88025_TOKENRING";
		}
		else if (ifType == 10)
		{
			return "IF_TYPE_ISO88026_MAN";
		}
		else if (ifType == 11)
		{
			return "IF_TYPE_STARLAN";
		}
		else if (ifType == 12)
		{
			return "IF_TYPE_PROTEON_10MBIT";
		}
		else if (ifType == 13)
		{
			return "IF_TYPE_PROTEON_80MBIT";
		}
		else if (ifType == 14)
		{
			return "IF_TYPE_HYPERCHANNEL";
		}
		else if (ifType == 15)
		{
			return "IF_TYPE_FDDI";
		}
		else if (ifType == 16)
		{
			return "IF_TYPE_LAP_B";
		}
		else if (ifType == 17)
		{
			return "IF_TYPE_SDLC";
		}
		else if (ifType == 18)
		{
			return "IF_TYPE_DS1 (DS1-MIB)";
		}
		else if (ifType == 19)
		{
			return "IF_TYPE_E1 (Obsolete; see DS1-MIB)";
		}
		else if (ifType == 20)
		{
			return "IF_TYPE_BASIC_ISDN";
		}
		else if (ifType == 21)
		{
			return "IF_TYPE_PRIMARY_ISDN";
		}
		else if (ifType == 22)
		{
			return "IF_TYPE_PROP_POINT2POINT_SERIAL (proprietary serial)";
		}
		else if (ifType == 23)
		{
			return "IF_TYPE_PPP";
		}
		else if (ifType == 24)
		{
			return "IF_TYPE_SOFTWARE_LOOPBACK";
		}
		else if (ifType == 25)
		{
			return "IF_TYPE_EON (CLNP over IP)";
		}
		else if (ifType == 26)
		{
			return "IF_TYPE_ETHERNET_3MBIT";
		}
		else if (ifType == 27)
		{
			return "IF_TYPE_NSIP (XNS over IP)";
		}
		else if (ifType == 28)
		{
			return "IF_TYPE_SLIP (Generic Slip)";
		}
		else if (ifType == 29)
		{
			return "IF_TYPE_ULTRA (ULTRA Technologies)";
		}
		else if (ifType == 30)
		{
			return "IF_TYPE_DS3 (DS3-MIB)";
		}
		else if (ifType == 31)
		{
			return "IF_TYPE_SIP (SMDS, coffee)";
		}
		else if (ifType == 32)
		{
			return "IF_TYPE_FRAMERELAY (DTE only)";
		}
		else if (ifType == 33)
		{
			return "IF_TYPE_RS232";
		}
		else if (ifType == 34)
		{
			return "IF_TYPE_PARA (Parallel port)";
		}
		else if (ifType == 35)
		{
			return "IF_TYPE_ARCNET";
		}
		else if (ifType == 36)
		{
			return "IF_TYPE_ARCNET_PLUS";
		}
		else if (ifType == 37)
		{
			return "IF_TYPE_ATM (ATM cells)";
		}
		else if (ifType == 38)
		{
			return "IF_TYPE_MIO_X25";
		}
		else if (ifType == 39)
		{
			return "IF_TYPE_SONET (SONET or SDH)";
		}
		else if (ifType == 40)
		{
			return "IF_TYPE_X25_PLE";
		}
		else if (ifType == 41)
		{
			return "IF_TYPE_ISO88022_LLC";
		}
		else if (ifType == 42)
		{
			return "IF_TYPE_LOCALTALK";
		}
		else if (ifType == 43)
		{
			return "IF_TYPE_SMDS_DXI";
		}
		else if (ifType == 44)
		{
			return "IF_TYPE_FRAMERELAY_SERVICE (FRNETSERV-MIB)";
		}
		else if (ifType == 45)
		{
			return "IF_TYPE_V35";
		}
		else if (ifType == 46)
		{
			return "IF_TYPE_HSSI";
		}
		else if (ifType == 47)
		{
			return "IF_TYPE_HIPPI";
		}
		else if (ifType == 48)
		{
			return "IF_TYPE_MODEM (Generic Modem)";
		}
		else if (ifType == 49)
		{
			return "IF_TYPE_AAL5 (AAL5 over ATM)";
		}
		else if (ifType == 50)
		{
			return "IF_TYPE_SONET_PATH";
		}
	}
	else if (ifType > 50 && ifType <= 100)
	{
		if (ifType == 51)
		{
			return "IF_TYPE_SONET_VT";
		}
		else if (ifType == 52)
		{
			return "IF_TYPE_SMDS_ICIP (SMDS InterCarrier Interface)";
		}
		else if (ifType == 53)
		{
			return "IF_TYPE_PROP_VIRTUAL (Proprietary virtual/internal)";
		}
		else if (ifType == 54)
		{
			return "IF_TYPE_PROP_MULTIPLEXOR (Proprietary multiplexing)";
		}
		else if (ifType == 55)
		{
			return "IF_TYPE_IEEE80212 (100BaseVG)";
		}
		else if (ifType == 56)
		{
			return "IF_TYPE_FIBRECHANNEL";
		}
		else if (ifType == 57)
		{
			return "IF_TYPE_HIPPIINTERFACE";
		}
		else if (ifType == 58)
		{
			return "IF_TYPE_FRAMERELAY_INTERCONNECT (Obsolete, use IF_TYPE_FRAMERELAY (DTE only) or IF_TYPE_FRAMERELAY_SERVICE (FRNETSERV-MIB))";
		}
		else if (ifType == 59)
		{
			return "IF_TYPE_AFLANE_8023 (ATM Emulated LAN for 802.3)";
		}
		else if (ifType == 60)
		{
			return "IF_TYPE_AFLANE_8025 (ATM Emulated LAN for 802.5)";
		}
		else if (ifType == 61)
		{
			return "IF_TYPE_CCTEMUL (ATM Emulated circuit)";
		}
		else if (ifType == 62)
		{
			return "IF_TYPE_FASTETHER (Fast Ethernet (100BaseT))";
		}
		else if (ifType == 63)
		{
			return "IF_TYPE_ISDN (ISDN and X.25)";
		}
		else if (ifType == 64)
		{
			return "IF_TYPE_V11 (CCITT V.11/X.21)";
		}
		else if (ifType == 65)
		{
			return "IF_TYPE_V36 (CCITT V.36)";
		}
		else if (ifType == 66)
		{
			return "IF_TYPE_G703_64K (CCITT G703 at 64Kbps)";
		}
		else if (ifType == 67)
		{
			return "IF_TYPE_G703_2MB (Obsolete; see DS1-MIB)";
		}
		else if (ifType == 68)
		{
			return "IF_TYPE_QLLC (SNA QLLC)";
		}
		else if (ifType == 69)
		{
			return "IF_TYPE_FASTETHER_FX (Fast Ethernet (100BaseFX))";
		}
		else if (ifType == 70)
		{
			return "IF_TYPE_CHANNEL";
		}
		else if (ifType == 71)
		{
			return "IF_TYPE_IEEE80211 (Radio spread spectrum)";
		}
		else if (ifType == 72)
		{
			return "IF_TYPE_IBM370PARCHAN (IBM System 360/370 OEMI Channel)";
		}
		else if (ifType == 73)
		{
			return "IF_TYPE_ESCON (IBM Enterprise Systems Connection)";
		}
		else if (ifType == 74)
		{
			return "IF_TYPE_DLSW (Data Link Switching)";
		}
		else if (ifType == 75)
		{
			return "IF_TYPE_ISDN_S (ISDN S/T interface)";
		}
		else if (ifType == 76)
		{
			return "IF_TYPE_ISDN_U (ISDN U interface)";
		}
		else if (ifType == 77)
		{
			return "IF_TYPE_LAP_D (Link Access Protocol D)";
		}
		else if (ifType == 78)
		{
			return "IF_TYPE_IPSWITCH (IP Switching Objects)";
		}
		else if (ifType == 79)
		{
			return "IF_TYPE_RSRB (Remote Source Route Bridging)";
		}
		else if (ifType == 80)
		{
			return "IF_TYPE_ATM_LOGICAL (ATM Logical Port)";
		}
		else if (ifType == 81)
		{
			return "IF_TYPE_DS0 (Digital Signal Level 0)";
		}
		else if (ifType == 82)
		{
			return "IF_TYPE_DS0_BUNDLE (Group of ds0s on the same ds1)";
		}
		else if (ifType == 83)
		{
			return "IF_TYPE_BSC (Bisynchronous Protocol)";
		}
		else if (ifType == 84)
		{
			return "IF_TYPE_ASYNC (Asynchronous Protocol)";
		}
		else if (ifType == 85)
		{
			return "IF_TYPE_CNR (Combat Net Radio)";
		}
		else if (ifType == 86)
		{
			return "IF_TYPE_ISO88025R_DTR (ISO 802.5r DTR)";
		}
		else if (ifType == 87)
		{
			return "IF_TYPE_EPLRS (Ext Pos Loc Report Sys)";
		}
		else if (ifType == 88)
		{
			return "IF_TYPE_ARAP (AppleTalk Remote Access Protocol)";
		}
		else if (ifType == 89)
		{
			return "IF_TYPE_PROP_CNLS (Proprietary Connectionless Protocol)";
		}
		else if (ifType == 90)
		{
			return "IF_TYPE_HOSTPAD (CCITT-ITU X.29 PAD Protocol)";
		}
		else if (ifType == 91)
		{
			return "IF_TYPE_TERMPAD (CCITT-ITU X.3 PAD Facility)";
		}
		else if (ifType == 92)
		{
			return "IF_TYPE_FRAMERELAY_MPI (Multiprotocol Interconnect over FR)";
		}
		else if (ifType == 93)
		{
			return "IF_TYPE_X213 (CCITT-ITU X213)";
		}
		else if (ifType == 94)
		{
			return "IF_TYPE_ADSL (Asymmetric Digital Subscriber Loop)";
		}
		else if (ifType == 95)
		{
			return "IF_TYPE_RADSL (Rate-Adapt Digital Subscriber Loop)";
		}
		else if (ifType == 96)
		{
			return "IF_TYPE_SDSL (Symmetric Digital Subscriber Loop)";
		}
		else if (ifType == 97)
		{
			return "IF_TYPE_VDSL (Very H-Speed Digital Subscriber Loop)";
		}
		else if (ifType == 98)
		{
			return "IF_TYPE_ISO88025_CRFPRINT (ISO 802.5 CRFP)";
		}
		else if (ifType == 99)
		{
			return "IF_TYPE_MYRINET (Myricom Myrinet)";
		}
		else if (ifType == 100)
		{
			return "IF_TYPE_VOICE_EM (Voice recEive and transMit)";
		}
	}
	else if (ifType > 100 && ifType <= 150)
	{
		if (ifType == 101)
		{
			return "IF_TYPE_VOICE_FXO (Voice Foreign Exchange Office)";
		}
		else if (ifType == 102)
		{
			return "IF_TYPE_VOICE_FXS (Voice Foreign Exchange Station)";
		}
		else if (ifType == 103)
		{
			return "IF_TYPE_VOICE_ENCAP (Voice encapsulation)";
		}
		else if (ifType == 104)
		{
			return "IF_TYPE_VOICE_OVERIP (Voice over IP encapsulation)";
		}
		else if (ifType == 105)
		{
			return "IF_TYPE_ATM_DXI (ATM DXI)";
		}
		else if (ifType == 106)
		{
			return "IF_TYPE_ATM_FUNI (ATM FUNI)";
		}
		else if (ifType == 107)
		{
			return "IF_TYPE_ATM_IMA (ATM IMA)";
		}
		else if (ifType == 108)
		{
			return "IF_TYPE_PPPMULTILINKBUNDLE (PPP Multilink Bundle)";
		}
		else if (ifType == 109)
		{
			return "IF_TYPE_IPOVER_CDLC (IBM ipOverCdlc)";
		}
		else if (ifType == 110)
		{
			return "IF_TYPE_IPOVER_CLAW (IBM Common Link Access to Workstation)";
		}
		else if (ifType == 111)
		{
			return "IF_TYPE_STACKTOSTACK (IBM stackToStack)";
		}
		else if (ifType == 112)
		{
			return "IF_TYPE_VIRTUALIPADDRESS (IBM VIPA)";
		}
		else if (ifType == 113)
		{
			return "IF_TYPE_MPC (IBM multi-proto channel support)";
		}
		else if (ifType == 114)
		{
			return "IF_TYPE_IPOVER_ATM (IBM ipOverAtm)";
		}
		else if (ifType == 115)
		{
			return "IF_TYPE_ISO88025_FIBER (ISO 802.5j Fiber Token Ring)";
		}
		else if (ifType == 116)
		{
			return "IF_TYPE_TDLC (IBM Twinaxial data link control)";
		}
		else if (ifType == 117)
		{
			return "IF_TYPE_GIGABITETHERNET";
		}
		else if (ifType == 118)
		{
			return "IF_TYPE_HDLC";
		}
		else if (ifType == 119)
		{
			return "IF_TYPE_LAP_F";
		}
		else if (ifType == 120)
		{
			return "IF_TYPE_V37";
		}
		else if (ifType == 121)
		{
			return "IF_TYPE_X25_MLP (Multi-Link Protocol)";
		}
		else if (ifType == 122)
		{
			return "IF_TYPE_X25_HUNTGROUP (X.25 Hunt Group)";
		}
		else if (ifType == 123)
		{
			return "IF_TYPE_TRANSPHDLC";
		}
		else if (ifType == 124)
		{
			return "IF_TYPE_INTERLEAVE (Interleave channel)";
		}
		else if (ifType == 125)
		{
			return "IF_TYPE_FAST (Fast channel)";
		}
		else if (ifType == 126)
		{
			return "IF_TYPE_IP (IP for APPN HPR in IP networks)";
		}
		else if (ifType == 127)
		{
			return "IF_TYPE_DOCSCABLE_MACLAYER (CATV Mac Layer)";
		}
		else if (ifType == 128)
		{
			return "IF_TYPE_DOCSCABLE_DOWNSTREAM (CATV Downstream interface)";
		}
		else if (ifType == 129)
		{
			return "IF_TYPE_DOCSCABLE_UPSTREAM (CATV Upstream interface)";
		}
		else if (ifType == 130)
		{
			return "IF_TYPE_A12MPPSWITCH (Avalon Parallel Processor)";
		}
		else if (ifType == 131)
		{
			return "IF_TYPE_TUNNEL (Encapsulation interface)";
		}
		else if (ifType == 132)
		{
			return "IF_TYPE_COFFEE (Coffee pot)";
		}
		else if (ifType == 133)
		{
			return "IF_TYPE_CES (Circuit Emulation Service)";
		}
		else if (ifType == 134)
		{
			return "IF_TYPE_ATM_SUBINTERFACE (ATM Sub Interface)";
		}
		else if (ifType == 135)
		{
			return "IF_TYPE_L2_VLAN (Layer 2 Virtual LAN using 802.1Q)";
		}
		else if (ifType == 136)
		{
			return "IF_TYPE_L3_IPVLAN (Layer 3 Virtual LAN using IP)";
		}
		else if (ifType == 137)
		{
			return "IF_TYPE_L3_IPXVLAN (Layer 3 Virtual LAN using IPX)";
		}
		else if (ifType == 138)
		{
			return "IF_TYPE_DIGITALPOWERLINE (IP over Power Lines)";
		}
		else if (ifType == 139)
		{
			return "IF_TYPE_MEDIAMAILOVERIP (Multimedia Mail over IP)";
		}
		else if (ifType == 140)
		{
			return "IF_TYPE_DTM (Dynamic synchronous Transfer Mode)";
		}
		else if (ifType == 141)
		{
			return "IF_TYPE_DCN (Data Communications Network)";
		}
		else if (ifType == 142)
		{
			return "IF_TYPE_IPFORWARD (IP Forwarding Interface)";
		}
		else if (ifType == 143)
		{
			return "IF_TYPE_MSDSL (Multi-rate Symmetric DSL)";
		}
		else if (ifType == 144)
		{
			return "IF_TYPE_IEEE1394 (IEEE1394 High Perf Serial Bus)";
		}
		else if (ifType == 145)
		{
			return "IF_TYPE_IF_GSN";
		}
		else if (ifType == 146)
		{
			return "IF_TYPE_DVBRCC_MACLAYER";
		}
		else if (ifType == 147)
		{
			return "IF_TYPE_DVBRCC_DOWNSTREAM";
		}
		else if (ifType == 148)
		{
			return "IF_TYPE_DVBRCC_UPSTREAM";
		}
		else if (ifType == 149)
		{
			return "IF_TYPE_ATM_VIRTUAL";
		}
		else if (ifType == 150)
		{
			return "IF_TYPE_MPLS_TUNNEL";
		}
	}
	else if (ifType > 150 && ifType <= 200)
	{
		if (ifType == 151)
		{
			return "IF_TYPE_SRP";
		}
		else if (ifType == 152)
		{
			return "IF_TYPE_VOICEOVERATM";
		}
		else if (ifType == 153)
		{
			return "IF_TYPE_VOICEOVERFRAMERELAY";
		}
		else if (ifType == 154)
		{
			return "IF_TYPE_IDSL";
		}
		else if (ifType == 155)
		{
			return "IF_TYPE_COMPOSITELINK";
		}
		else if (ifType == 156)
		{
			return "IF_TYPE_SS7_SIGLINK";
		}
		else if (ifType == 157)
		{
			return "IF_TYPE_PROP_WIRELESS_P2P";
		}
		else if (ifType == 158)
		{
			return "IF_TYPE_FR_FORWARD";
		}
		else if (ifType == 159)
		{
			return "IF_TYPE_RFC1483";
		}
		else if (ifType == 160)
		{
			return "IF_TYPE_USB";
		}
		else if (ifType == 161)
		{
			return "IF_TYPE_IEEE8023AD_LAG";
		}
		else if (ifType == 162)
		{
			return "IF_TYPE_BGP_POLICY_ACCOUNTING";
		}
		else if (ifType == 163)
		{
			return "IF_TYPE_FRF16_MFR_BUNDLE";
		}
		else if (ifType == 164)
		{
			return "IF_TYPE_H323_GATEKEEPER";
		}
		else if (ifType == 165)
		{
			return "IF_TYPE_H323_PROXY";
		}
		else if (ifType == 166)
		{
			return "IF_TYPE_MPLS";
		}
		else if (ifType == 167)
		{
			return "IF_TYPE_MF_SIGLINK";
		}
		else if (ifType == 168)
		{
			return "IF_TYPE_HDSL2";
		}
		else if (ifType == 169)
		{
			return "IF_TYPE_SHDSL";
		}
		else if (ifType == 170)
		{
			return "IF_TYPE_DS1_FDL";
		}
		else if (ifType == 171)
		{
			return "IF_TYPE_POS";
		}
		else if (ifType == 172)
		{
			return "IF_TYPE_DVB_ASI_IN";
		}
		else if (ifType == 173)
		{
			return "IF_TYPE_DVB_ASI_OUT";
		}
		else if (ifType == 174)
		{
			return "IF_TYPE_PLC";
		}
		else if (ifType == 175)
		{
			return "IF_TYPE_NFAS";
		}
		else if (ifType == 176)
		{
			return "IF_TYPE_TR008";
		}
		else if (ifType == 177)
		{
			return "IF_TYPE_GR303_RDT";
		}
		else if (ifType == 178)
		{
			return "IF_TYPE_GR303_IDT";
		}
		else if (ifType == 179)
		{
			return "IF_TYPE_ISUP";
		}
		else if (ifType == 180)
		{
			return "IF_TYPE_PROP_DOCS_WIRELESS_MACLAYER";
		}
		else if (ifType == 181)
		{
			return "IF_TYPE_PROP_DOCS_WIRELESS_DOWNSTREAM";
		}
		else if (ifType == 182)
		{
			return "IF_TYPE_PROP_DOCS_WIRELESS_UPSTREAM";
		}
		else if (ifType == 183)
		{
			return "IF_TYPE_HIPERLAN2";
		}
		else if (ifType == 184)
		{
			return "IF_TYPE_PROP_BWA_P2MP";
		}
		else if (ifType == 185)
		{
			return "IF_TYPE_SONET_OVERHEAD_CHANNEL";
		}
		else if (ifType == 186)
		{
			return "IF_TYPE_DIGITAL_WRAPPER_OVERHEAD_CHANNEL";
		}
		else if (ifType == 187)
		{
			return "IF_TYPE_AAL2";
		}
		else if (ifType == 188)
		{
			return "IF_TYPE_RADIO_MAC";
		}
		else if (ifType == 189)
		{
			return "IF_TYPE_ATM_RADIO";
		}
		else if (ifType == 190)
		{
			return "IF_TYPE_IMT";
		}
		else if (ifType == 191)
		{
			return "IF_TYPE_MVL";
		}
		else if (ifType == 192)
		{
			return "IF_TYPE_REACH_DSL";
		}
		else if (ifType == 193)
		{
			return "IF_TYPE_FR_DLCI_ENDPT";
		}
		else if (ifType == 194)
		{
			return "IF_TYPE_ATM_VCI_ENDPT";
		}
		else if (ifType == 195)
		{
			return "IF_TYPE_OPTICAL_CHANNEL";
		}
		else if (ifType == 196)
		{
			return "IF_TYPE_OPTICAL_TRANSPORT";
		}
	}
	else
	{
		if (ifType == 237)
		{
			return "IF_TYPE_IEEE80216_WMAN";
		}
		else if (ifType == 243)
		{
			return "IF_TYPE_WWANPP (WWAN devices based on GSM technology)";
		}
		else if (ifType == 244)
		{
			return "IF_TYPE_WWANPP2 (WWAN devices based on CDMA technology)";
		}
	}

	return "MAX_IF_TYPE";
}

bool WindowsNetworkInfo::WriteInfoToFile(const char* fileName)
{
	FILE* theFile;
	char* networkInfo;
	size_t networkInfoLen;
	bool retVal = false;

	if (fileName != NULL && strlen(fileName) > 0)
	{
		networkInfo = WindowsNetworkInfo::GetInfo();
		if (networkInfo != NULL)
		{
			networkInfoLen = strlen(networkInfo) + 1;

			if (fopen_s(&theFile, fileName, "w+") == 0)
			{
				fwrite(networkInfo, sizeof(*networkInfo), networkInfoLen, theFile);

				fclose(theFile);
			}

			delete networkInfo;
			retVal = true;
		}
	}

	return retVal;
}