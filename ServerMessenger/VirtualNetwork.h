#pragma once

#include "User.h"

class VirtualNetwork
{
public:

	std::weak_ptr<std::vector<User>> GetUsers();

	bool operator==(const VirtualNetwork& virtualNetwork) const = default;

private:

	std::shared_ptr<std::vector<User>> m_users;

};
