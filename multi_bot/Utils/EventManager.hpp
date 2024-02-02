#pragma once

#include <string>
#include <utility>
#include <unordered_map>

#include <sigslot/signal.hpp>

namespace utils {

template<typename ...Params>
class EventManager {
public:

    // c++ magic
    // returns true if successful
    template<typename ...FParams>
    // params = argument passed to sigslot.connect
    bool Register(const std::string id, FParams&&... params) {
        if (m_registered_event.find(id) != m_registered_event.end()) { return false; }
        m_registered_event.insert_or_assign(id, m_event.connect(std::forward<FParams>(params)...));
        return true;
    }

    void Remove(const std::string& id) {
        try {
            m_registered_event.at(id).disconnect();
            m_registered_event.erase(id);
        }
        catch (...) {
            return;
        }
    }

    void RemoveAll() {
        std::for_each(m_registered_event.begin(), m_registered_event.end(), [&](const auto &item) {
            Remove(item.first);
        });
    }

    void Invoke(Params... params) { m_event(std::forward<Params>(params)...); }

private:
    sigslot::signal<Params...> m_event;
    std::unordered_map<std::string, sigslot::connection> m_registered_event;
};
}
