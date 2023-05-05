#include "ServerMessenger.h"
#include "StringMessage.h"

StringMessage::StringMessage(std::string data) : Message<std::string>(data)
{

}

std::unique_ptr<char[]> StringMessage::Serialize() const
{
    std::unique_ptr<char[]> buffer(new char[m_data.size() + 1]);
    strcpy_s(buffer.get(), m_data.size() + 1, m_data.c_str());
    return buffer;
}

StringMessage StringMessage::Deserialize(const char* buffer)
{
    std::string data(buffer);
    return StringMessage(data);
}
