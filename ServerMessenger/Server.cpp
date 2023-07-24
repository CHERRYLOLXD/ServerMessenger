#include "stdafx.h"

#include "Server.h"

SOCKET Server::m_serverSocket = INVALID_SOCKET;
size_t Server::m_maxConnections = 3;
std::atomic_bool Server::m_isStopped = true;
std::condition_variable Server::m_conditionVariable;
std::mutex Server::m_serverMutex;
std::vector<Connection> Server::m_connections;

void Server::Start()
{
    Console::PrintLine(L"Start Server");
    m_isStopped = false;
    WinsockInitializer winsockInitializer;
    {
        if (winsockInitializer.Initialize() != 0)
        {
            Console::PrintErrorLine(L"WSAStartup failed");
            Stop();
            return;
        }
        if ((m_serverSocket = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
        {
            Console::PrintErrorLine(std::format(L"socket failed: [{}]", WSAGetLastError()));
            Stop();
            return;
        }
        SOCKADDR_IN6 localAddress{};
        localAddress.sin6_addr = in6addr_any;
        localAddress.sin6_family = AF_INET6;
        localAddress.sin6_port = htons(static_cast<unsigned short>(std::stoi(Console::ReadLine(L"Select the port [enter 0 for automatic assignment]: "))));
        int addressLength = sizeof(localAddress);
        if (bind(m_serverSocket, (SOCKADDR*)&localAddress, addressLength) == SOCKET_ERROR)
        {
            Console::PrintErrorLine(std::format(L"bind failed: [{}]", WSAGetLastError()));
            Stop();
            return;
        }
        if (listen(m_serverSocket, SOMAXCONN) == SOCKET_ERROR)
        {
            Console::PrintErrorLine(std::format(L"listen failed: [{}]", WSAGetLastError()));
            Stop();
            return;
        }
        std::unique_ptr<IP_ADAPTER_ADDRESSES> adapterAddresses;
        ULONG outputBufferLength{};
        ULONG family = localAddress.sin6_family;
        ULONG result;
        result = GetAdaptersAddresses(family, GAA_FLAG_INCLUDE_PREFIX, nullptr, nullptr, &outputBufferLength);
        if (result == ERROR_BUFFER_OVERFLOW)
        {
            adapterAddresses.reset(new IP_ADAPTER_ADDRESSES[outputBufferLength]);
        }
        result = GetAdaptersAddresses(family, GAA_FLAG_INCLUDE_PREFIX, nullptr, adapterAddresses.get(), &outputBufferLength);
        if (result != NO_ERROR)
        {
            Console::PrintErrorLine(std::format(L"GetAdaptersAddresses failed: [{}]", result));
            Stop();
            return;
        }
        for (PIP_ADAPTER_ADDRESSES pCurrAddresses = adapterAddresses.get(); pCurrAddresses; pCurrAddresses = pCurrAddresses->Next)
        {
            for (PIP_ADAPTER_UNICAST_ADDRESS pUnicast = pCurrAddresses->FirstUnicastAddress; pUnicast; pUnicast = pUnicast->Next)
            {
                SOCKADDR_IN6* address = reinterpret_cast<SOCKADDR_IN6*>(pUnicast->Address.lpSockaddr);
                char addressBuffer[INET6_ADDRSTRLEN];
                inet_ntop(AF_INET6, &address->sin6_addr, addressBuffer, INET6_ADDRSTRLEN);
                std::wstring_view friendlyName(pCurrAddresses->FriendlyName);
                Console::PrintLine(std::vformat(L"You may be able to connect to the server through this adapter: [{}] at this IP: [{}]", std::make_wformat_args(friendlyName, Console::DataToWString(addressBuffer))));
            }
        }
        if (getsockname(m_serverSocket, (SOCKADDR*)&localAddress, &addressLength) == SOCKET_ERROR)
        {
            Console::PrintErrorLine(std::format(L"getsockname failed: [{}]", WSAGetLastError()));
            Stop();
            return;
        }
        Console::PrintLine(std::format(L"Listening on this port: [{}]", ntohs(localAddress.sin6_port)));
    }
	while (!m_isStopped)
	{
		std::unique_lock<std::mutex> lock(m_serverMutex);
		m_conditionVariable.wait(lock, []() { return m_connections.size() < m_maxConnections || m_isStopped; });
		if (!m_isStopped)
		{
			m_connections.emplace_back(Connection()).Start();
		}
	}
}

void Server::Stop()
{
    Console::PrintLine(L"Stop Server");
	m_isStopped = true;
	m_conditionVariable.notify_all();
    CleanUpServer();
}

void Server::CleanUpServer()
{
    m_connections.clear();
    if (m_serverSocket != INVALID_SOCKET)
    {
        closesocket(m_serverSocket);
    }
}

SOCKET Server::GetSocket()
{
    return m_serverSocket;
}

void Server::RemoveConnection(const Connection& connection)
{
    std::vector<Connection>::iterator iterator;
    if ((iterator = std::find(m_connections.begin(), m_connections.end(), connection)) >= m_connections.end())
    {
        return;
    }
    m_connections.erase(iterator);
    if (!m_isStopped)
    {
        m_conditionVariable.notify_all();
    }
}
