#pragma once

#include "User.h"

class VirtualNetwork
{
public:

	std::vector<User>& GetUsers();

	bool operator==(const VirtualNetwork& virtualNetwork) const = default;

private:

	std::vector<User> m_users;

};
