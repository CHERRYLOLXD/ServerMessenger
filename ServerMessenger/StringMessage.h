#pragma once

#include "Message.h"

class StringMessage : public Message<std::wstring>
{
public:

    StringMessage(const std::wstring& data);
    virtual std::unique_ptr<std::vector<char>> Serialize() const override;
    static StringMessage Deserialize(const std::vector<char>& buffer);

};
