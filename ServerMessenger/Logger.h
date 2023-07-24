#pragma once

class Logger final
{
public:

    Logger(std::wstring_view logFileName, uint64_t maxLogSize);
    ~Logger();

    void Log(std::wstring&& message);

private:

    void RotateLogFiles();
    void WriteLogsToFile();

    std::atomic_bool m_stopWriting = false;
    std::condition_variable m_conditionVar;
    std::jthread m_logQueueThread = std::jthread(&Logger::WriteLogsToFile, this);
    std::mutex m_mutex;
    std::queue<std::wstring> m_activeLogQueue;
    std::queue<std::wstring> m_writeLogQueue;
    std::wofstream m_logFile;
    std::wstring m_logFileName;
    uint64_t m_currentLogSize = 0;
    uint64_t m_maxLogSize;

};
