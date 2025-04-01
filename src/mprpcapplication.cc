#include "./include/mprpcapplication.h"
#include <iostream>
#include <unistd.h>
#include "mprpcapplication.h"

MpRpcConfig MpRpcApplication::m_config;

void ShowArgsHelp()
{
    std::cout<<"format: command -i <configfile>" << std::endl;
}

void MpRpcApplication::Init(int argc, char **argv)
{
    if (argc < 2)
    {
        ShowArgsHelp();
        exit(EXIT_FAILURE);
    }

    int opt = 0;
    std::string config_file;
    // getopt 函数用于解析命令行参数，它按照 optstring 中定义的格式来解析 argv 数组中的参数，并返回当前处理的选项字符存于opt中。
    while ((opt = getopt(argc, argv, "i:")) != -1)
    {
        switch (opt)
        {
        case 'i':
            // optarg是一个全局变量 ,用于存储 getopt 函数解析到的命令行选项的参数值。
            config_file = optarg;
            break;
        case '?':
            ShowArgsHelp();
            exit(EXIT_FAILURE);
        case ':':
            ShowArgsHelp();
            exit(EXIT_FAILURE);
        default:
            break;
        }
    }
    //开始加载配置文件
    m_config.LoadConfigFile(config_file.c_str());
    // std::cout << "rpcserverip:" << m_config.Load("rpcserverip") << std::endl;
    // std::cout << "rpcserverport:" << m_config.Load("rpcserverport") << std::endl;
    // std::cout << "zookeeperip:" << m_config.Load("zookeeperip") << std::endl;
    // std::cout << "zookeeperport:" << m_config.Load("zookeeperport") << std::endl;
}

MpRpcApplication &MpRpcApplication::GetInstance()
{
    static MpRpcApplication app;
    return app;
}
MpRpcConfig &MpRpcApplication::GetConfig()
{
    return m_config;
}