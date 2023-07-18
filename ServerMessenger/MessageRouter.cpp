#include "ServerMessenger.h"
#include "MessageRouter.h"

std::vector<VirtualNetwork> MessageRouter::m_virtualNetworks;

void MessageRouter::SendMessageToVirtualNetwork(const VirtualNetwork& virtualNetworks)
{
    virtualNetworks;
}

void MessageRouter::AddVirtualNetwork(VirtualNetwork& virtualNetwork)
{
	m_virtualNetworks.push_back(virtualNetwork);
}

void MessageRouter::RemoveVirtualNetwork(VirtualNetwork& virtualNetwork)
{
    std::vector<VirtualNetwork>::iterator iterator;
    if ((iterator = std::find(m_virtualNetworks.begin(), m_virtualNetworks.end(), virtualNetwork)) >= m_virtualNetworks.end())
    {
        return;
    }
    m_virtualNetworks.erase(iterator);
}
