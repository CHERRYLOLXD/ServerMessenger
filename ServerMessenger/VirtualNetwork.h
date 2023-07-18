#pragma once

#include "User.h"

class VirtualNetwork
{
public:

	void AddUser(User* user);
	void RemoveUser(User* user);
	bool operator==(const VirtualNetwork& virtualNetwork) const = default;

private:

	std::vector<User*> m_users;

};
