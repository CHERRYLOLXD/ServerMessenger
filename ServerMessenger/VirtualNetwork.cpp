#include "ServerMessenger.h"
#include "VirtualNetwork.h"

void VirtualNetwork::AddUser(User* user)
{
	m_users.push_back(user);
}

void VirtualNetwork::RemoveUser(User* user)
{
    std::vector<User*>::iterator iterator;
    if ((iterator = std::find(m_users.begin(), m_users.end(), user)) >= m_users.end())
    {
        return;
    }
    m_users.erase(iterator);
}
