#pragma once
#include <unordered_map>
#include <string>

class MpRpcConfig
{
public:
    // 加载配置文件
    void LoadConfigFile(const char *config_file);
    // 查询配置信息
    std::string Load(const std::string &key);

private:
    std::unordered_map<std::string, std::string> m_configMap;
    // 去掉字符串前后的空格
    void Trim(std::string &src_buf);
};