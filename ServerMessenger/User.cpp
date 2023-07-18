#include "ServerMessenger.h"
#include "User.h"

std::wstring User::GetIdentifier()
{
	return m_identifier;
}

void User::SetIdentifier(std::wstring identifier)
{
	m_identifier = identifier;
}

std::wstring User::GetName()
{
	return m_name;
}

void User::SetName(std::wstring name)
{
	m_name = name;
}
