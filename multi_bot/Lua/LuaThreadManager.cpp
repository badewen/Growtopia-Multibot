#include "LuaThreadManager.h"

#include <iostream>

void LuaThreadManager::LockThreadList() {
    m_thread_list_mtx.lock();
}

void LuaThreadManager::RemoveRunningThread(int th_id) {
    m_running_thread_list.erase(th_id);
}

void LuaThreadManager::AddThread(int th_id, std::thread th) {
    m_running_thread_list.insert_or_assign(th_id, std::move(th));
}

void LuaThreadManager::CleanupDeadThread() {
    std::unordered_map<int, std::thread> active_thread{};

    for (auto& th_kv : m_running_thread_list) {
        if (th_kv.second.joinable()) {
            active_thread.insert_or_assign(th_kv.first, std::move(th_kv.second));
        }
    }
    m_running_thread_list = std::move(active_thread);
}

void LuaThreadManager::UnlockThreadList() {
    m_thread_list_mtx.unlock();
}

void LuaThreadManager::WaitAllThread() {
    LockThreadList();
    for (auto& thread : m_running_thread_list) {
        if (thread.second.joinable()) {
            thread.second.join();
        }
    }
    UnlockThreadList();
}
