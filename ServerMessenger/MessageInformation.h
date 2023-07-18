#pragma once

class MessageInformation
{
public:

    enum MessagesTypes
    {
        StringMessage
    };

    MessagesTypes GetMessageType();
    void SetMessageType(MessagesTypes messageType);
    int32_t GetMessageSize();
    void SetMessageSize(int32_t messageSize);

private:

    MessagesTypes m_messageType;
    int32_t m_messageSize;

};
