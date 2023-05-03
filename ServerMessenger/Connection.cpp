#include "ServerMessenger.h"
#include "Server.h"
#include "Connection.h"

Connection::Connection() : m_clientSocket(INVALID_SOCKET)
{
    std::thread t(&Connection::Start, *this);
    t.detach();
}

void Connection::Start()
{
	spdlog::get("server")->info("Start Connection");

    sockaddr_in6 clientInfo{};
    int clientInfoSize = sizeof(clientInfo);

    m_clientSocket = accept(Server::GetSocket(), (SOCKADDR*)&clientInfo, &clientInfoSize);
    if (m_clientSocket == INVALID_SOCKET)
    {
        spdlog::get("server")->error("accept failed: {}", WSAGetLastError());
        Server::RemoveConnection(*this);
        return;
    }

    char clientAddr[INET6_ADDRSTRLEN];
    inet_ntop(AF_INET6, &clientInfo.sin6_addr, clientAddr, INET6_ADDRSTRLEN);

    spdlog::get("server")->info("Client connected from {}", clientAddr);

    std::string welcomeMessage = "Welcome to the server!";
    if (send(m_clientSocket, welcomeMessage.c_str(), welcomeMessage.size(), 0) == SOCKET_ERROR)
    {
        spdlog::get("server")->error("send failed: {}", WSAGetLastError());
        Stop();
        return;
    }
}

void Connection::Stop()
{
    spdlog::get("server")->info("Stop Connection");
    closesocket(m_clientSocket);
    Server::RemoveConnection(*this);
}
