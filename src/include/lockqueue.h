#pragma once
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>

template <typename T>
class LockQueue
{
public:
    void push(const T &data)
    {
        std::lock_guard<std::mutex> lock(m_mutex); // 加锁
        m_queue.push(data);                        // 将数据压入队列
        m_condvariable.notify_one();               // 通知等待的线程
    }
    T pop()
    {
        std::unique_lock<std::mutex> lock(m_mutex); // 加锁
        while (m_queue.empty())                     // 等待队列非空
        {
            m_condvariable.wait(lock); // 等待条件变量
        }
        T data = m_queue.front(); // 获取队列头部数据
        m_queue.pop();            // 弹出队列头部
        return data;
    }

    bool empty()
    {
        return m_queue.empty();
    }

private:
    std::queue<T> m_queue;
    std::mutex m_mutex;
    std::condition_variable m_condvariable;
};
