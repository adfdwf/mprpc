#pragma once
#include "lockqueue.h"
#include <fstream>
#include <string>

// 定义宏 LOG_INFO("xxx %d %s", 20, "xxxx")
#define LOG_INFO(logmsgformat, ...)                     \
    do                                                  \
    {                                                   \
        Logger &logger = Logger::GetInstance();         \
        logger.setLogLevel(INFO);                       \
        char c[1024] = {0};                             \
        snprintf(c, 1024, logmsgformat, ##__VA_ARGS__); \
        logger.log(c);                                  \
    } while (0)

#define LOG_ERR(logmsgformat, ...)                      \
    do                                                  \
    {                                                   \
        Logger &logger = Logger::GetInstance();         \
        logger.setLogLevel(ERROR);                      \
        char c[1024] = {0};                             \
        snprintf(c, 1024, logmsgformat, ##__VA_ARGS__); \
        logger.log(c);                                  \
    } while (0)

enum LogLevel
{
    INFO,
    ERROR
};

class Logger
{
public:
    // 获取日志的单例
    static Logger &GetInstance();
    // 设置日志级别
    void setLogLevel(LogLevel level);
    // 写日志
    void log(std::string msg);

private:
    int m_logLevel;
    LockQueue<std::string> m_lckQue;
    bool m_exitFlag;
    std::thread m_workerThread;
    std::mutex m_exitMutex;
    std::condition_variable m_exitCond;
    std::ofstream m_logFile; // 日志文件流
    Logger();
    ~Logger();
    Logger(const Logger &) = delete;
    Logger(const Logger &&) = delete;

    void workerThreadFunc();
};