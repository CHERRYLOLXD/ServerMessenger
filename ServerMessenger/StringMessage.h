#pragma once

#include "Message.h"

class StringMessage : public Message<std::wstring>
{
public:

    StringMessage(const std::wstring& data);

    virtual std::shared_ptr<std::vector<char>> Serialize() const override;
    static StringMessage Deserialize(std::shared_ptr<std::vector<char>> buffer);

};
