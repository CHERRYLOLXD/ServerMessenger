#pragma once

class User
{
public:

	std::wstring GetIdentifier();
	void SetIdentifier(std::wstring identifier);
	std::wstring GetName();
	void SetName(std::wstring name);

private:

	std::wstring m_identifier;
	std::wstring m_name;
	std::wstring m_password;

};
