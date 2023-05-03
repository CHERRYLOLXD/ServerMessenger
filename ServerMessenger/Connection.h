#pragma once

class Connection
{
public:

	Connection();

	void Start();

	void Stop();

private:

	SOCKET m_clientSocket;

};
