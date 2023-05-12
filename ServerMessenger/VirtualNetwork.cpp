#include "ServerMessenger.h"
#include "VirtualNetwork.h"

std::weak_ptr<std::vector<User>> VirtualNetwork::GetUsers()
{
	return m_users;
}
