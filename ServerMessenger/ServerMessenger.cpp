#include "ServerMessenger.h"
#include "Console.h"
#include "Server.h"
#include "VirtualNetwork.h"

int32_t wmain()
{
    Console::PrintLine(L"Start application");
    {
        int32_t result;
        result = _setmode(_fileno(stdout), _O_U16TEXT);
        if (result < 0)
        {
            Console::PrintErrorLine(L"_setmode stdout failed: {}", result);
            return result;
        }
        result = _setmode(_fileno(stderr), _O_U16TEXT);
        if (result < 0)
        {
            Console::PrintErrorLine(L"_setmode stderr failed: {}", result);
            return result;
        }
        result = _setmode(_fileno(stdin), _O_U16TEXT);
        if (result < 0)
        {
            Console::PrintErrorLine(L"_setmode stdin failed: {}", result);
            return result;
        }
    }
    std::jthread serverJThread(Server::Start);
    while (true)
    {
        if (GetAsyncKeyState(VK_MENU) & KEY_PRESSED)
        {
            if (GetAsyncKeyState(VK_UP) & KEY_PRESSED)
            {
                Console::ReadLine();
            }
        }
    }
    Server::Stop();
    Console::PrintLine(L"Stop application");
}
