#include "ServerMessenger.h"
#include "ServerNetwork.h"

std::vector<VirtualNetwork> ServerNetwork::m_virtualNetworks;

VirtualNetwork& ServerNetwork::GetVirtualNetwork()
{
    return m_virtualNetworks.front();
}

void ServerNetwork::AddVirtualNetwork(const VirtualNetwork& virtualNetwork)
{
	m_virtualNetworks.emplace_back(virtualNetwork);
}

void ServerNetwork::RemoveVirtualNetwork(const VirtualNetwork& virtualNetwork)
{
    std::vector<VirtualNetwork>::iterator it;
    if ((it = std::find(m_virtualNetworks.begin(), m_virtualNetworks.end(), virtualNetwork)) >= m_virtualNetworks.end())
    {
        return;
    }
    m_virtualNetworks.erase(it);
}
