#include "ServerMessenger.h"
#include "Console.h"

std::mutex Console::m_printConsoleMutex;
std::mutex Console::m_readConsoleMutex;

std::wstring Console::ReadLine()
{
    std::wstring line;
    std::lock_guard readConsoleLock(m_readConsoleMutex);
    std::getline(std::wcin, line);
    return std::move(line);
}
