#pragma once
#include "mprpcconfig.h"
// mprpc框架的基础类，负责框架的一些初始化操作，如通过命令行指定配置文件，利用单例模式全局管理配置
class MpRpcApplication{
    public:
        static void Init(int argc, char **argv);
        static MpRpcApplication &GetInstance();
        static MpRpcConfig &GetConfig();

    private:
        static MpRpcConfig m_config;
        MpRpcApplication() {}
        MpRpcApplication(const MpRpcApplication &) = delete;
        MpRpcApplication(MpRpcApplication &&) = delete;
};
