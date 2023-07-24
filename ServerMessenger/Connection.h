#pragma once

#include "Console.h"
#include "MessageInformation.h"
#include "Server.h"
#include "StringMessage.h"

class Connection final
{
public:

	~Connection();

	void Start();
	void Stop();

	template<class T>
	void Send(const Message<T> message)
	{

	}

	bool operator==(const Connection& connection) const = default;

private:

	void Receive();

	void CleanUpConnection();

	SOCKET m_clientSocket = INVALID_SOCKET;
	std::jthread m_connectionThread = std::jthread(&Connection::Receive, this);

};
