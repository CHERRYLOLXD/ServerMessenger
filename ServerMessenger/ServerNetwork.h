#pragma once

#include "VirtualNetwork.h"

class ServerNetwork
{
public:

	static VirtualNetwork& GetVirtualNetwork();
	static void AddVirtualNetwork(const VirtualNetwork& virtualNetwork);
	static void RemoveVirtualNetwork(const VirtualNetwork& virtualNetwork);

private:

	static std::vector<VirtualNetwork> m_virtualNetworks;

};
