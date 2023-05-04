#include "ServerMessenger.h"
#include "Server.h"
#include "Connection.h"

Connection::~Connection()
{
    m_isDestroyed = true;
}

void Connection::Start()
{
	spdlog::get("server")->info("Start Connection");

    sockaddr_in6 clientInfo{};
    int clientInfoSize = sizeof(clientInfo);

    if ((m_clientSocket = accept(Server::GetSocket(), (SOCKADDR*)&clientInfo, &clientInfoSize)) == INVALID_SOCKET)
    {
        spdlog::get("server")->error("accept failed: {}", WSAGetLastError());
        Stop();
        return;
    }

    char clientAddr[INET6_ADDRSTRLEN];
    inet_ntop(AF_INET6, &clientInfo.sin6_addr, clientAddr, INET6_ADDRSTRLEN);

    spdlog::get("server")->info("Client connected from {}", clientAddr);
}

void Connection::Stop()
{
    spdlog::get("server")->info("Stop Connection");
    if (!m_isDestroyed)
    {
        CleanUpConnection();
        Server::RemoveConnection(this);
    }
}

void Connection::CleanUpConnection()
{
    if (m_clientSocket != INVALID_SOCKET)
    {
        closesocket(m_clientSocket);
    }
}
