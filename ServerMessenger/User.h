#pragma once

class User
{
public:

	std::string GetId();
	void SetId(std::string id);

	std::string GetName();
	void SetName(std::string name);

private:

	std::string m_id;

	std::string m_name;

};
