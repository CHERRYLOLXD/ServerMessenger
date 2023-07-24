#pragma once

#include "Logger.h"

class Console final
{
public:

    static void Initialize();

    static std::string WStringToData(const wchar_t* data);
    static std::wstring DataToWString(const char* data);

    static std::wstring ReadLine(std::wstring_view message);

    static void PrintErrorLine(std::wstring_view message);
    static void PrintLine(std::wstring_view message);

private:

    static void LogMessage(std::wstring&& message);
    static void SetMode(int fileDescriptor);

    static std::mutex m_printConsoleMutex;
    static std::mutex m_readConsoleMutex;

};
