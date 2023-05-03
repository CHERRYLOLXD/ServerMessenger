#include "ServerMessenger.h"
#include "Server.h"

std::vector<Connection> Server::m_connections;
size_t Server::m_maxConnections = 10;

bool Server::m_isStop = false;

std::mutex Server::m_serverMutex;
std::condition_variable Server::m_conditionVariable;

SOCKET Server::m_serverSocket;

void Server::Start()
{
	spdlog::get("server")->info("Start Server");

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        spdlog::get("server")->error("WSAStartup failed");
        return;
    }

    SOCKET m_serverSocket = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
    if (m_serverSocket == INVALID_SOCKET)
    {
        spdlog::get("server")->error("socket failed: {}", WSAGetLastError());
        WSACleanup();
        return;
    }

    sockaddr_in6 localAddr{};
    localAddr.sin6_addr = in6addr_any;
    localAddr.sin6_family = AF_INET6;
    localAddr.sin6_port = htons(0);
    int addrLen = sizeof(localAddr);

    if (bind(m_serverSocket, (sockaddr*)&localAddr, addrLen) == SOCKET_ERROR)
    {
        spdlog::get("server")->error("bind failed: {}", WSAGetLastError());
        closesocket(m_serverSocket);
        WSACleanup();
        return;
    }

    if (getsockname(m_serverSocket, (sockaddr*)&localAddr, &addrLen) == SOCKET_ERROR)
    {
        spdlog::get("server")->error("getsockname failed: {}", WSAGetLastError());
        closesocket(m_serverSocket);
        WSACleanup();
        return;
    }

    spdlog::get("server")->info("Listening on port {}", ntohs(localAddr.sin6_port));

    if (listen(m_serverSocket, SOMAXCONN) == SOCKET_ERROR)
    {
        spdlog::get("server")->error("listen failed: {}", WSAGetLastError());
        closesocket(m_serverSocket);
        WSACleanup();
        return;
    }

	while (!m_isStop)
	{
		std::unique_lock<std::mutex> lock(m_serverMutex);
		m_conditionVariable.wait(lock, []() { return m_connections.size() < m_maxConnections || m_isStop; });
		if (!m_isStop)
		{
			m_connections.emplace_back(Connection());
		}
	}

    closesocket(m_serverSocket);
    WSACleanup();
}

void Server::Stop()
{
    spdlog::get("server")->info("Stop Server");
	m_isStop = true;
	m_conditionVariable.notify_all();
	for (Connection& connection : m_connections)
	{
		connection.Stop();
	}
	m_connections.clear();
}

SOCKET Server::GetSocket()
{
    return m_serverSocket;
}

void Server::RemoveConnection(const Connection& connection)
{
    //m_connections.erase(std::find(m_connections.begin(), m_connections.end(), []() {} ));
}
