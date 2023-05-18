#include "ServerMessenger.h"
#include "MessageRouter.h"

std::vector<VirtualNetwork> MessageRouter::m_virtualNetworks;

void MessageRouter::SendMessageToVirtualNetwork(const VirtualNetwork& virtualNetworks)
{
    virtualNetworks;
}

VirtualNetwork& MessageRouter::GetVirtualNetwork()
{
    return m_virtualNetworks.front();
}

void MessageRouter::AddVirtualNetwork(const VirtualNetwork& virtualNetwork)
{
	m_virtualNetworks.emplace_back(virtualNetwork);
}

void MessageRouter::RemoveVirtualNetwork(const VirtualNetwork& virtualNetwork)
{
    std::vector<VirtualNetwork>::iterator iterator;
    if ((iterator = std::find(m_virtualNetworks.begin(), m_virtualNetworks.end(), virtualNetwork)) >= m_virtualNetworks.end())
    {
        return;
    }
    m_virtualNetworks.erase(iterator);
}
