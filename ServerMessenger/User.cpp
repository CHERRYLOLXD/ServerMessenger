#include "ServerMessenger.h"
#include "User.h"

std::wstring User::GetId()
{
	return m_id;
}

void User::SetId(std::wstring id)
{
	m_id = id;
}

std::wstring User::GetName()
{
	return m_name;
}

void User::SetName(std::wstring name)
{
	m_name = name;
}
