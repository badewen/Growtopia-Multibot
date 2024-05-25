#pragma once

#include <thread>
#include <mutex>
#include <unordered_map>

class LuaThreadManager
{
public:
    void LockThreadList();

    void RemoveRunningThread(int th_id);
    void AddThread(int th_id, std::thread th);

    // this filters out any thread that is not joinable.
    void CleanupDeadThread();

    void UnlockThreadList();

    void WaitAllThread();

private:
    std::mutex m_thread_list_mtx{};
    std::unordered_map<int, std::thread> m_running_thread_list{};
};

