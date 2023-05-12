#pragma once

namespace MessageInfo
{
    enum MessageTypes
    {
        StringMessage
    };
};

template <class T>
class Message
{
public:

    Message(const T& data) : m_data(data)
    {

    }

    T& GetData()
    {
        return m_data;
    }
    void SetData(const T& data)
    {
        m_data = data;
    }

    virtual std::shared_ptr<std::vector<char>> Serialize() const
    {
        std::shared_ptr<std::vector<char>> buffer(new std::vector<char>(sizeof(T)));
        memcpy_s(buffer->data(), sizeof(T), &m_data, sizeof(T));
        return buffer;
    }
    static Message<T> Deserialize(std::shared_ptr<std::vector<char>> buffer)
    {
        T data{};
        memcpy_s(&data, sizeof(T), buffer->data(), sizeof(T));
        return Message<T>(std::move(data));
    }

protected:

    T m_data;

};
