#include "ServerMessenger.h"
#include "User.h"

std::string User::GetId()
{
	return m_id;
}

void User::SetId(std::string id)
{
	m_id = id;
}

std::string User::GetName()
{
	return m_name;
}

void User::SetName(std::string name)
{
	m_name = name;
}
