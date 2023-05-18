#pragma once

#include "Connection.h"

class Server
{
public:

	static void Start();
	static void Stop();

	static SOCKET GetSocket();

	static void RemoveConnection(const Connection& connection);

private:

	static void CleanUpServer();

	static std::vector<Connection> m_connections;
	static size_t m_maxConnections;

	static bool m_isStopped;

	static std::mutex m_serverMutex;
	static std::condition_variable m_conditionVariable;
	
	static SOCKET m_serverSocket;

};
