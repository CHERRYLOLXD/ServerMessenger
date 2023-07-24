#include "stdafx.h"

#include "Connection.h"

Connection::~Connection()
{
    CleanUpConnection();
    Console::PrintLine(L"Stop Connection");
}

void Connection::Start()
{
    Console::PrintLine(L"Start Connection");
    SOCKADDR_IN6 clientInfo{};
    int clientInfoSize = sizeof(clientInfo);
    if ((m_clientSocket = accept(Server::GetSocket(), (SOCKADDR*)&clientInfo, &clientInfoSize)) == INVALID_SOCKET)
    {
        Console::PrintErrorLine(std::format(L"accept failed: [{}]", WSAGetLastError()));
        Stop();
        return;
    }
    char addressBuffer[INET6_ADDRSTRLEN];
    inet_ntop(AF_INET6, &clientInfo.sin6_addr, addressBuffer, sizeof(addressBuffer));
    Console::PrintLine(std::vformat(L"Client connected from: [{}]", std::make_wformat_args(Console::DataToWString(addressBuffer))));
}

void Connection::Stop()
{
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
    Console::PrintLine(L"Receive messages");
    while (this)
    {
        std::vector<char> messageInformationBuffer(sizeof(MessageInformation));
        int bytesReceived = recv(m_clientSocket, messageInformationBuffer.data(), sizeof(MessageInformation), 0);
        if (bytesReceived != sizeof(MessageInformation))
        {
            Console::PrintErrorLine(std::format(L"recv messageInformation failed: [{}]", WSAGetLastError()));
            Stop();
            return;
        }
        MessageInformation messageInformation = Message<MessageInformation>::Deserialize(messageInformationBuffer).GetData();
        MessageInformation::MessagesTypes messageType = messageInformation.GetMessageType();
        int messageSize = messageInformation.GetMessageSize();
        std::vector<char> dataBuffer(messageSize);
        bytesReceived = recv(m_clientSocket, dataBuffer.data(), messageSize, 0);
        if (bytesReceived != messageSize)
        {
            Console::PrintErrorLine(std::format(L"recv data failed: [{}] received bytes | [{}] bytes", bytesReceived, messageSize));
            Stop();
            return;
        }
        switch (messageType)
        {
        case MessageInformation::StringMessage:
        {
            Console::PrintLine(std::vformat(L"StringMessage: [{}]", std::make_wformat_args(StringMessage::Deserialize(dataBuffer).GetData())));
            break;
        }
        default:
        {
            Console::PrintErrorLine(std::vformat(L"unknown messageType: [{}]", std::make_wformat_args(static_cast<int>(messageType))));
            Stop();
            return;
        }
        }
    }
}
