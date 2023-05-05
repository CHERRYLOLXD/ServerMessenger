#pragma once

#include "Message.h"

class StringMessage : public Message<std::string>
{
public:

    StringMessage(std::string data);

    virtual std::unique_ptr<char[]> Serialize() const override;
    static StringMessage Deserialize(const char* buffer);

};
