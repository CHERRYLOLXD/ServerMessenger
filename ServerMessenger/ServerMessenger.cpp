#include "stdafx.h"

#include "ServerMessenger.h"

int wmain()
{
    Console::Initialize();
    Console::PrintLine(L"Start application");
    {
        std::jthread serverThread(&Server::Start);
    }
    Console::PrintLine(L"Stop application");
}
