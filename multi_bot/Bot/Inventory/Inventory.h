#pragma once

#include <unordered_map>

class Inventory
{
public:
    using ItemID    = uint16_t;
    using ItemCount = uint16_t;
public:
    void SetInventorySpace(uint32_t inv_space);

    void SetItemAmount(uint16_t item_id, uint16_t amount);
    void AddItem(uint16_t item_id, uint16_t amount);
    void RemoveItem(uint16_t item_id, uint16_t amount);

    // reset all stored items
    void ResetInventoryItems();

    uint16_t GetItemAmount(uint16_t item_id);
    uint32_t GetAvailInvSpace();

    const std::unordered_map<ItemID, ItemCount>& GetInventory() { return m_item_list; }
    uint32_t GetTotalInvSpace() { return m_inventory_space; }

private:
    uint32_t m_inventory_space;
    std::unordered_map<ItemID, ItemCount> m_item_list;
};