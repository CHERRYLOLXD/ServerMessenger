#include "ServerMessenger.h"
#include "Console.h"
#include "Server.h"
#include "Connection.h"

Connection::~Connection()
{
    Console::PrintLine(L"Stop Connection");

    CleanUpConnection();
}

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

    char addressBuffer[INET6_ADDRSTRLEN];
    inet_ntop(AF_INET6, &clientInfo.sin6_addr, addressBuffer, sizeof(addressBuffer));
    Console::PrintLine(L"Client connected from: {}", addressBuffer);

    std::thread connectionThread(&Connection::Receive, *this);
    connectionThread.detach();
}

void Connection::Stop()
{
    if (this)
    {
        Server::RemoveConnection(*this);
    }
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
        std::vector<char> messageInformationBuffer(sizeof(MessageInformation));
        int32_t bytesReceived = recv(m_clientSocket, messageInformationBuffer.data(), sizeof(uint8_t), 0);
        if (bytesReceived != sizeof(uint8_t))
        {
            Console::PrintErrorLine(L"recv messageInformation failed: {}", WSAGetLastError());
            Stop();
            return;
        }
        MessageInformation messageInformation = Message<MessageInformation>::Deserialize(messageInformationBuffer).GetData();

        MessageInformation::MessagesTypes messageType = messageInformation.GetMessageType();
        int32_t messageSize = messageInformation.GetMessageSize();

        std::vector<char> dataBuffer(messageSize);
        bytesReceived = recv(m_clientSocket, dataBuffer.data(), messageSize, 0);
        if (bytesReceived != messageSize)
        {
            Console::PrintErrorLine(L"recv data failed: {}", WSAGetLastError());
            Stop();
            return;
        }

        switch (messageType)
        {
        case MessageInformation::StringMessage:
        {
            Console::PrintLine(L"StringMessage: {}", StringMessage::Deserialize(dataBuffer).GetData());
            break;
        }
        default:
        {
            Console::PrintErrorLine(L"unknown messageType: {}", messageType);
            Stop();
            return;
        }
        }
    }
}
