#include "BotManager.h"

std::shared_ptr<Bot> BotManager::AddBot(std::string bot_id, std::shared_ptr<ILogger> logger) {
    m_bot_list.insert_or_assign(bot_id, std::make_shared<Bot>(logger));

    return GetBot(bot_id);
}

std::shared_ptr<Bot> BotManager::GetBot(std::string bot_id) {
    auto found = m_bot_list.find(bot_id);

    return found != m_bot_list.end() ? found->second : nullptr;
}

void BotManager::RemoveBot(std::string bot_id) {
    m_bot_list.erase(bot_id);
}
