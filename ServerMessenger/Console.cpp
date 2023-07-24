#include "stdafx.h"

#include "Console.h"

std::mutex Console::m_printConsoleMutex;
std::mutex Console::m_readConsoleMutex;

void Console::Initialize()
{
    SetMode(_fileno(stderr));
    SetMode(_fileno(stdin));
    SetMode(_fileno(stdout));
}

std::string Console::WStringToData(const wchar_t* data)
{
    int dataLen = static_cast<int>(wcslen(data));
    int mDataLen = WideCharToMultiByte(CP_UTF8, 0, data, dataLen, nullptr, 0, nullptr, nullptr);
    std::string dataString(mDataLen, '\0');
    WideCharToMultiByte(CP_UTF8, 0, data, dataLen, &dataString[0], mDataLen, nullptr, nullptr);
    return std::move(dataString);
}

std::wstring Console::DataToWString(const char* data)
{
    int dataLen = static_cast<int>(strlen(data));
    int wDataLen = MultiByteToWideChar(CP_UTF8, 0, data, dataLen, nullptr, 0);
    std::wstring dataWString(wDataLen, L'\000');
    MultiByteToWideChar(CP_UTF8, 0, data, dataLen, &dataWString[0], wDataLen);
    return std::move(dataWString);
}

std::wstring Console::ReadLine(std::wstring_view message)
{
    std::lock_guard printConsoleLock(m_printConsoleMutex);
    std::wcout << message << std::endl;
    LogMessage(std::wstring(message));
    std::wstring line;
    std::lock_guard readConsoleLock(m_readConsoleMutex);
    std::getline(std::wcin, line);
    LogMessage(std::wstring(line));
    return std::move(line);
}

void Console::PrintErrorLine(std::wstring_view message)
{
    std::lock_guard printConsoleLock(m_printConsoleMutex);
    std::wcerr << message << std::endl;
    LogMessage(std::wstring(message));
}

void Console::PrintLine(std::wstring_view message)
{
    std::lock_guard printConsoleLock(m_printConsoleMutex);
    std::wcout << message << std::endl;
    LogMessage(std::wstring(message));
}

void Console::LogMessage(std::wstring&& message)
{
    static Logger logger(L"console.log", 16 * 1024 * 1024);
    logger.Log(std::move(message));
}

void Console::SetMode(int fileDescriptor)
{
    int result = _setmode(fileDescriptor, _O_U16TEXT);
    if (result < 0)
    {
        throw std::runtime_error(std::format("_setmode [{}] failed: [{}]", fileDescriptor, result));
    }
}
