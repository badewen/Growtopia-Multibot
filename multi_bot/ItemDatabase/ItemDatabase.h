#pragma once

#include <string>
#include <unordered_map>

#include "ItemInfo.h"
#include "../Utils/BinaryStreamReader.h"

class ItemDatabase
{
public:
    using ItemID = uint32_t;
public:
    // false if no file is found or other error occurred.
    bool LoadFromFile(const std::string& file_name);
    void LoadFromReader(Utils::BinaryStreamReader& reader);

    std::string DecryptString(ItemID item_id, const std::vector<uint8_t>& buffer);

    ItemInfo GetItemInfo(uint32_t item_id);

private:
    ItemInfo parse_next_item_info(Utils::BinaryStreamReader& reader);

private:
    uint16_t m_items_dat_version{};
    uint32_t m_items_count{};
    std::unordered_map<ItemID, ItemInfo> m_item_info_map{};
};

