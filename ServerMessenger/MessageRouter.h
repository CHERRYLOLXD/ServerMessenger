#pragma once

#include "VirtualNetwork.h"

class MessageRouter
{
public:

	static void SendMessageToVirtualNetwork(const VirtualNetwork& virtualNetworks);
	static void AddVirtualNetwork(VirtualNetwork& virtualNetwork);
	static void RemoveVirtualNetwork(VirtualNetwork& virtualNetwork);

private:

	static std::vector<VirtualNetwork>  m_virtualNetworks;

};
