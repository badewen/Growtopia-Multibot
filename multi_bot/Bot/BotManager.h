#pragma once

#include <unordered_map>

#include "Bot.h"

class BotManager
{
public:
    std::shared_ptr<Bot> AddBot(std::string bot_id, std::shared_ptr<ILogger> logger);
    std::shared_ptr<Bot> GetBot(std::string bot_id);
    void RemoveBot(std::string bot_id);

    void SetItemDatabase(std::shared_ptr<ItemDatabase> item_dats) { m_item_dats = std::move(item_dats); }

    std::unordered_map<std::string /*bot_id*/, std::shared_ptr<Bot>>& GetBotList() { return m_bot_list; }

public:
    std::unordered_map<std::string /*bot_id*/, std::shared_ptr<Bot>> m_bot_list;
    std::shared_ptr<ItemDatabase> m_item_dats;
};


