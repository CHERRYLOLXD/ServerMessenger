#include "stdafx.h"

#include "Logger.h"

Logger::Logger(std::wstring_view logFileName, uint64_t maxLogSize) :
    m_logFileName(logFileName),
    m_maxLogSize(maxLogSize)
{
    m_logFile.imbue(std::locale("en_US.UTF-8"));
}

Logger::~Logger()
{
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_stopWriting = true;
    }
    m_conditionVar.notify_one();
}

void Logger::Log(std::wstring&& message)
{
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_activeLogQueue.push(std::move(message));
    }
    m_conditionVar.notify_one();
}

void Logger::RotateLogFiles()
{
    int i = 1;
    std::wstring extension = L".log";
    std::wstring newFileName;
    do
    {
        newFileName = std::format(L"{}{}{}", m_logFileName.substr(0, m_logFileName.length() - extension.length()), i++, extension);
    } while (std::filesystem::exists(newFileName));
    std::filesystem::rename(m_logFileName, newFileName);
}

void Logger::WriteLogsToFile()
{
    std::queue<std::wstring>* activeQueue = &m_activeLogQueue;
    std::queue<std::wstring>* writeQueue = &m_writeLogQueue;
    while (!m_stopWriting)
    {
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_conditionVar.wait(lock, [this]
                {
                    return !m_activeLogQueue.empty() || m_stopWriting;
                });
            std::swap(activeQueue, writeQueue);
        }
        if (!writeQueue->empty())
        {
            m_logFile.open(std::filesystem::path(m_logFileName), std::ios::app);
            if (m_logFile.is_open())
            {
                while (!writeQueue->empty())
                {
                    std::wstring logMessage = writeQueue->front();
                    uint64_t messageSize = logMessage.size() * sizeof(wchar_t);
                    if (m_currentLogSize + messageSize >= m_maxLogSize)
                    {
                        m_logFile.close();
                        RotateLogFiles();
                        m_logFile.open(m_logFileName, std::ios::app);
                        m_currentLogSize = 0;
                    }
                    m_currentLogSize += messageSize;
                    m_logFile << logMessage << std::endl;
                    writeQueue->pop();
                }
                m_logFile.close();
            }
        }
    }
}
