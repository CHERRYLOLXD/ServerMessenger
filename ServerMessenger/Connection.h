#pragma once

class Connection
{
public:

	~Connection();

	void Start();

	void Stop();

	void CleanUpConnection();

	bool operator==(const Connection& connection) const = default;

private:

	SOCKET m_clientSocket = INVALID_SOCKET;

	bool m_isDestroyed = false;

};
