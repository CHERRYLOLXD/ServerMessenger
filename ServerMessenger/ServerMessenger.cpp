#include "ServerMessenger.h"
#include "Console.h"
#include "Server.h"
#include "VirtualNetwork.h"

int32_t wmain()
{
    std::wcout << std::unitbuf;

    int32_t result;
    if (result = _setmode(_fileno(stdout), _O_U16TEXT) < 0)
    {
        Console::PrintErrorLine(L"_setmode failed: {}", result);
        return result;
    }
    if (result = _setmode(_fileno(stdin), _O_U16TEXT) < 0)
    {
        Console::PrintErrorLine(L"_setmode failed: {}", result);
        return result;
    }
    if (result = _setmode(_fileno(stderr), _O_U16TEXT) < 0)
    {
        Console::PrintErrorLine(L"_setmode failed: {}", result);
        return result;
    }

    std::jthread serverJThread(Server::Start);
    while (true)
    {

    }
    Server::Stop();
}
