#pragma once

class User
{
public:

	std::wstring GetId();
	void SetId(std::wstring id);

	std::wstring GetName();
	void SetName(std::wstring name);

private:

	std::wstring m_id;

	std::wstring m_name;
	std::wstring m_password;

};
