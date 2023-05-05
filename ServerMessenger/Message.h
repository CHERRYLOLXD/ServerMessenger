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

    Message(const T data) : m_data(data)
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

    virtual std::unique_ptr<char[]> Serialize() const
    {
        std::unique_ptr<char[]> buffer(new char[sizeof(T)]);
        memcpy_s(buffer.get(), sizeof(T), &m_data, sizeof(T));
        return buffer;
    }
    static Message<T> Deserialize(const char* buffer)
    {
        T data{};
        memcpy_s(&data, sizeof(T), buffer, sizeof(T));
        return Message<T>(std::move(data));
    }

protected:

    T m_data;

};
