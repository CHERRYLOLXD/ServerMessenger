#include "ServerMessenger.h"
#include "StringMessage.h"

StringMessage::StringMessage(const std::wstring& data) : Message<std::wstring>(data)
{

}

std::shared_ptr<std::vector<char>> StringMessage::Serialize() const
{
    size_t size = m_data.size() * sizeof(wchar_t);
    std::shared_ptr<std::vector<char>> buffer = std::make_shared<std::vector<char>>(size);
    memcpy_s(buffer->data(), size, m_data.data(), size);
    return buffer;
}

StringMessage StringMessage::Deserialize(std::shared_ptr<std::vector<char>> buffer)
{
    size_t size = buffer->size() / sizeof(wchar_t);
    std::wstring data(reinterpret_cast<wchar_t*>(buffer->data()), size);
    return StringMessage(data);
}
