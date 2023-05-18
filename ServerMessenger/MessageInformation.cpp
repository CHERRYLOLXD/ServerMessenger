#include "ServerMessenger.h"
#include "MessageInformation.h"

MessageInformation::MessagesTypes MessageInformation::GetMessageType()
{
	return m_messageType;
}

void MessageInformation::SetMessageType(MessagesTypes messageType)
{
	m_messageType = messageType;
}

int32_t MessageInformation::GetMessageSize()
{
	return m_messageSize;
}

void MessageInformation::SetMessageSize(int32_t messageSize)
{
	m_messageSize = messageSize;
}
