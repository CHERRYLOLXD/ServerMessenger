#pragma once

class Console {
public:

    template<class... Args>
    static void PrintLine(std::wstring_view message, const Args& ...args)
    {
        std::wstringstream formatted;
        FormatString(message, formatted, args...);
        std::lock_guard printConsoleLock(m_printConsoleMutex);
        std::wcout << std::move(formatted.str()) << std::endl;
    }
    template<class... Args>
    static void PrintErrorLine(std::wstring_view message, const Args& ...args)
    {
        std::wstringstream formatted;
        FormatString(message, formatted, args...);
        std::lock_guard printConsoleLock(m_printConsoleMutex);
        std::wcerr << std::move(formatted.str()) << std::endl;
    }

    static std::wstring ReadLine();

private:

    static void FormatString(std::wstring_view message, std::wstringstream& stream)
    {
        stream << message;
    }
    template<class T, class... Args>
    static void FormatString(std::wstring_view message, std::wstringstream& stream, const T& arg, const Args&... args)
    {
        size_t bracketsPosition = message.find(L"{}");
        if (bracketsPosition == std::wstring_view::npos)
        {
            stream << message;
        }
        else
        {
            stream << message.substr(0, bracketsPosition) << L"[" << arg << L"]";
            FormatString(message.substr(bracketsPosition + 2), stream, args...);
        }
    }

    static std::mutex m_printConsoleMutex;
    static std::mutex m_readConsoleMutex;

};
