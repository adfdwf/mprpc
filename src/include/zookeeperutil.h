#pragma once

#include <zookeeper/zookeeper.h>
#include <semaphore.h>
#include <string>

class ZkClient
{
public:
    ZkClient();
    ~ZkClient();
    // 启动 ZooKeeper 客户端，建立与 ZooKeeper 服务器的连接。
    void start();
    // 在 ZooKeeper 中创建一个节点。
    // state:节点的类型。如果为 1，则创建临时节点；否则创建持久节点。
    void create(const char *path, const char *data, int datalen, int state = 0);
    // 从 ZooKeeper 中获取指定节点的数据。
    std::string GetData(const char *path);

private:
    // ZooKeeper 的句柄，用于与 ZooKeeper 服务器进行交互,通过这个句柄，可以执行各种 ZooKeeper 操作，如创建节点、获取数据等。
    zhandle_t *m_zhandle;
};