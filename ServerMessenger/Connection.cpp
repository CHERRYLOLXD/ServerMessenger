#include "ServerMessenger.h"
#include "Console.h"
#include "Server.h"
#include "Connection.h"

void Connection::Start()
{
    Console::PrintLine(L"Start Connection");

    SOCKADDR_IN6 clientInfo{};
    int32_t clientInfoSize = sizeof(clientInfo);

    if ((m_clientSocket = accept(Server::GetSocket(), (SOCKADDR*)&clientInfo, &clientInfoSize)) == INVALID_SOCKET)
    {
        Console::PrintErrorLine(L"accept failed: {}", WSAGetLastError());
        Stop();
        return;
    }

    char addrBuffer[INET6_ADDRSTRLEN];
    inet_ntop(AF_INET6, &clientInfo.sin6_addr, addrBuffer, INET6_ADDRSTRLEN);
    Console::PrintLine(L"Client connected from: {}", addrBuffer);

    std::thread connectionThread(&Connection::Receive, *this);
    connectionThread.detach();
}

void Connection::Stop()
{
    Console::PrintLine(L"Stop Connection");
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
    Console::PrintLine(L"Receive Connection");

    while (this)
    {
        std::shared_ptr<std::vector<char>> idBuffer = std::make_shared<std::vector<char>>(sizeof(uint8_t));
        int32_t bytesReceived = recv(m_clientSocket, idBuffer->data(), sizeof(uint8_t), 0);
        if (bytesReceived != sizeof(uint8_t))
        {
            Console::PrintErrorLine(L"recv id failed");
            Stop();
            return;
        }
        uint8_t id = Message<uint8_t>::Deserialize(idBuffer).GetData();

        std::shared_ptr<std::vector<char>> sizeBuffer = std::make_shared<std::vector<char>>(sizeof(int32_t));
        bytesReceived = recv(m_clientSocket, sizeBuffer->data(), sizeof(int32_t), 0);
        if (bytesReceived != sizeof(int32_t))
        {
            Console::PrintErrorLine(L"recv size failed");
            Stop();
            return;
        }
        int32_t size = Message<int32_t>::Deserialize(sizeBuffer).GetData();

        std::shared_ptr<std::vector<char>> dataBuffer = std::make_shared<std::vector<char>>(size);
        bytesReceived = recv(m_clientSocket, dataBuffer->data(), size, 0);
        if (bytesReceived != size)
        {
            Console::PrintErrorLine(L"recv data failed");
            Stop();
            return;
        }

        switch (id)
        {
        case MessageInfo::StringMessage:
        {
            Console::PrintLine(L"StringMessage: {}", StringMessage::Deserialize(dataBuffer).GetData());
            break;
        }
        default:
        {
            Console::PrintErrorLine(L"id failed: ", id);
            Stop();
            return;
        }
        }
    }
    Stop();
}
