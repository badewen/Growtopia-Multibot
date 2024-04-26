#include "Inventory.h"

void Inventory::SetInventorySpace(uint32_t inv_space) {
    m_inventory_space = inv_space;
}

void Inventory::SetItemAmount(uint16_t item_id, uint16_t count) {
    m_item_list.insert_or_assign(item_id, count);
    if (m_item_list.find(item_id) != m_item_list.end()) {
        if (m_item_list[item_id] <= 0) {
            m_item_list.erase(item_id);
        }
    }
}

void Inventory::AddItem(uint16_t item_id, uint16_t amount) {
    SetItemAmount(item_id, GetItemAmount(item_id) + amount);
}

void Inventory::RemoveItem(uint16_t item_id, uint16_t amount) {
    SetItemAmount(item_id, GetItemAmount(item_id) - amount);
}

void Inventory::ResetInventoryItems() {
    m_item_list.clear();
}

void Inventory::Reset() {
    ResetInventoryItems();
    SetInventorySpace(0);
}

uint16_t Inventory::GetItemAmount(uint16_t item_id) {
    auto it = m_item_list.find(item_id);

    return it == m_item_list.end() ? 0 : it->second;
}

uint32_t Inventory::GetAvailInvSpace() {
    return GetTotalInvSpace() - m_item_list.size();
}


