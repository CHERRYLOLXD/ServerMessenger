#include "ServerMessenger.h"
#include "Server.h"

std::vector<Connection> Server::m_connections;
size_t Server::m_maxConnections = 1;

bool Server::m_isStart = false;
bool Server::m_isStop = true;
bool Server::m_isWinsockInitialized = false;

std::mutex Server::m_serverMutex;
std::condition_variable Server::m_conditionVariable;

SOCKET Server::m_serverSocket = INVALID_SOCKET;

void Server::Start()
{
    if (m_isStart)
    {
        spdlog::get("server")->info("Server has already been started");
        return;
    }
	spdlog::get("server")->info("Start Server");
    m_isStart = true;
    m_isStop = false;

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        spdlog::get("server")->error("WSAStartup failed");
        return;
    }
    m_isWinsockInitialized = true;

    if ((m_serverSocket = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
    {
        spdlog::get("server")->error("socket failed: {}", WSAGetLastError());
        CleanUpServer();
        return;
    }

    SOCKADDR_IN6 localAddr{};
    localAddr.sin6_addr = in6addr_any;
    localAddr.sin6_family = AF_INET6;
    localAddr.sin6_port = htons(12345);
    int addrLen = sizeof(localAddr);

    if (bind(m_serverSocket, (SOCKADDR*)&localAddr, addrLen) == SOCKET_ERROR)
    {
        spdlog::get("server")->error("bind failed: {}", WSAGetLastError());
        m_isStop = true;
        CleanUpServer();
        return;
    }

    if (getsockname(m_serverSocket, (SOCKADDR*)&localAddr, &addrLen) == SOCKET_ERROR)
    {
        spdlog::get("server")->error("getsockname failed: {}", WSAGetLastError());
        m_isStop = true;
        CleanUpServer();
        return;
    }

    spdlog::get("server")->info("Listening on [{}]:{}", "::", ntohs(localAddr.sin6_port));

    if (listen(m_serverSocket, SOMAXCONN) == SOCKET_ERROR)
    {
        spdlog::get("server")->error("listen failed: {}", WSAGetLastError());
        m_isStop = true;
        CleanUpServer();
        return;
    }

	while (!m_isStop)
	{
		std::unique_lock<std::mutex> lock(m_serverMutex);
		m_conditionVariable.wait(lock, []() { return m_connections.size() < m_maxConnections || m_isStop; });
		if (!m_isStop)
		{
			m_connections.emplace_back(Connection()).Start();
		}
	}
    m_isStart = false;
}

void Server::Stop()
{
    spdlog::get("server")->info("Stop Server");
	m_isStop = true;
	m_conditionVariable.notify_all();

    CleanUpServer();
}

void Server::CleanUpServer()
{
    if (m_serverSocket != INVALID_SOCKET)
    {
        closesocket(m_serverSocket);
    }
    if (m_isWinsockInitialized)
    {
        WSACleanup();
        m_isWinsockInitialized = false;
    }
    for (Connection& connection : m_connections)
    {
        connection.Stop();
    }
}

SOCKET Server::GetSocket()
{
    return m_serverSocket;
}

void Server::RemoveConnection(const Connection& connection)
{
    std::vector<Connection>::iterator it;
    if ((it = std::find(m_connections.begin(), m_connections.end(), connection)) == m_connections.end())
    {
        return;
    }
    m_connections.erase(it);
    if (!m_isStop)
    {
        m_conditionVariable.notify_all();
    }
}
