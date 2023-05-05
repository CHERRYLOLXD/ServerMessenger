#include "ServerMessenger.h"
#include "Server.h"
#include "Connection.h"

void Connection::Start()
{
	spdlog::get("server")->info("Start Connection");

    SOCKADDR_IN6 clientInfo{};
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

    std::thread connectionThread(&Connection::Receive, *this);
    connectionThread.detach();
}

void Connection::Stop()
{
    spdlog::get("server")->info("Stop Connection");
    CleanUpConnection();
    Server::RemoveConnection(*this);
}

void Connection::CleanUpConnection()
{
    if (m_clientSocket != INVALID_SOCKET)
    {
        closesocket(m_clientSocket);
    }
}

void Connection::Receive()
{
    while (this)
    {
        spdlog::get("server")->info("Receive Connection");
        char idBuffer[sizeof(uint8_t)];
        int bytesReceived = recv(m_clientSocket, idBuffer, sizeof(uint8_t), 0);
        if (bytesReceived != sizeof(uint8_t))
        {
            spdlog::get("server")->error("recv id failed: {}", WSAGetLastError());
            Stop();
            return;
        }
        uint8_t id = Message<uint8_t>::Deserialize(idBuffer).GetData();

        char sizeBuffer[sizeof(uint32_t)];
        bytesReceived = recv(m_clientSocket, sizeBuffer, sizeof(uint32_t), 0);
        if (bytesReceived != sizeof(uint32_t))
        {
            spdlog::get("server")->error("recv size failed: {}", WSAGetLastError());
            Stop();
            return;
        }
        uint32_t size = Message<uint32_t>::Deserialize(sizeBuffer).GetData();

        switch (id)
        {
        case MessageInfo::StringMessage:
        {
            std::unique_ptr<char[]> dataBuffer(new char[size]);
            bytesReceived = recv(m_clientSocket, dataBuffer.get(), size, 0);
            if (bytesReceived != size)
            {
                spdlog::get("server")->error("recv data failed: {}", WSAGetLastError());
                Stop();
                return;
            }
            spdlog::get("server")->info(StringMessage::Deserialize(dataBuffer.get()).GetData());
            break;
        }
        default:
        {
            spdlog::get("server")->error("type failed: {}", id);
            Stop();
            return;
        }
        }
    }
    Stop();
}
