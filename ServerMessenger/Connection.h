#pragma once

#include "MessageInformation.h"
#include "StringMessage.h"

class Connection
{
public:

	~Connection();

	void Start();
	void Stop();

	template<class T>
	void Send(const Message<T> message)
	{
		send(m_clientSocket, message.serialize().get(), sizeof(T), 0);
	}

	bool operator==(const Connection& connection) const = default;

private:

	void Receive();

	void CleanUpConnection();

	SOCKET m_clientSocket = INVALID_SOCKET;

};
