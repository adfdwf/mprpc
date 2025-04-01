#include "logger.h"
#include <iostream>
#include <ctime>
#include <string.h>


Logger &Logger::GetInstance()
{
    static Logger instance;
    return instance;
}

void Logger::setLogLevel(LogLevel level)
{
    m_logLevel = level;
}

void Logger::log(std::string msg)
{
    // 将日志消息放入队列
    m_lckQue.push(msg);
}

Logger::Logger() : m_logLevel(INFO), m_exitFlag(false)
{
    // 指定日志文件的目录
    const char *logDirectory = "/home/czl/Documents/Code/mprpc/log/"; // 修改为你的目标目录

    // 确保目录以斜杠结尾
    if (logDirectory[strlen(logDirectory) - 1] != '/')
    {
        std::cerr << "Log directory path should end with a slash ('/').";
        throw std::runtime_error("Invalid log directory path");
    }

    // 打开日志文件
    std::time_t now = std::time(nullptr);
    char filename[256];
    std::strftime(filename, sizeof(filename), "log_%Y%m%d_%H%M%S.log", std::localtime(&now));

    // 拼接完整路径
    std::string fullPath = std::string(logDirectory) + filename;

    // 打开日志文件
    m_logFile.open(fullPath, std::ios::out | std::ios::app);

    if (!m_logFile.is_open())
    {
        std::cerr << "Failed to open log file: " << fullPath << std::endl;
        throw std::runtime_error("Failed to open log file");
    }

    // 启动日志处理线程
    m_workerThread = std::thread(&Logger::workerThreadFunc, this);
}

Logger::~Logger()
{
    // 设置退出标志
    {
        std::lock_guard<std::mutex> lock(m_exitMutex);
        m_exitFlag = true;
    }
    // 通知日志处理线程
    m_exitCond.notify_one();
    // 等待日志处理线程退出
    if (m_workerThread.joinable())
    {
        m_workerThread.join();
    }
    // 关闭日志文件
    if (m_logFile.is_open())
    {
        m_logFile.close();
    }
}

void Logger::workerThreadFunc()
{
    while (true)
    {
        std::string logMsg;
        {
            std::unique_lock<std::mutex> lock(m_exitMutex);
            // 等待日志队列非空或退出标志被设置
            m_exitCond.wait(lock, [this]
                            { return !m_lckQue.empty() || m_exitFlag; });
            // 如果设置了退出标志且队列为空，则退出线程
            if (m_exitFlag && m_lckQue.empty())
            {
                break;
            }
            // 从队列中取出日志消息
            logMsg = m_lckQue.pop();
        }

        // 获取当前时间作为日志时间戳
        std::time_t now = std::time(nullptr);
        char timestamp[32];
        std::strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", std::localtime(&now));

        // 根据日志级别处理日志消息
        std::string logEntry;
        if (m_logLevel == INFO)
        {
            logEntry = std::string(timestamp) + " [INFO] " + logMsg + "\n";
        }
        else if (m_logLevel == ERROR)
        {
            logEntry = std::string(timestamp) + " [ERROR] " + logMsg + "\n";
        }

        // 写入日志文件
        if (m_logFile.is_open())
        {
            m_logFile << logEntry;
            m_logFile.flush(); // 立即刷新缓冲区
        }
        else
        {
            std::cerr << "Log file is not open. Message lost: " << logEntry;
        }
    }
}