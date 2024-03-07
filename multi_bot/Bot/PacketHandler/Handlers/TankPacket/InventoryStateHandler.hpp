#pragma once

#include "../../../Bot.h"
#include "../../IPacketHandler.hpp"
#include "../../../../Utils/BinaryStreamReader.h"

class InventoryStateHandler : public IPacketHandler
{
public:
    // good way to test BinaryReader
    // struct equivalent of the item infos in the SEND_INVENTORY packet
#pragma pack(push, 1)
    struct InvItemInfoPacketStruct {
        uint16_t ItemID, ItemAmount;
    };
#pragma pack(pop)

public:
    InventoryStateHandler(Bot* bot_obj) : IPacketHandler::IPacketHandler(bot_obj) {}

    void Handle(const TankPacket* tank_pkt) override {
        Utils::BinaryStreamReader reader{ tank_pkt->Value };

        // idk it is always 0x01 
        reader.Advance(1);

        uint32_t inventory_size = reader.GenericRead<uint32_t>();

        std::vector<InvItemInfoPacketStruct> items = reader.ReadList<uint16_t, InvItemInfoPacketStruct>();

        m_bot->GetInventoryRef().ResetInventoryItems();
        m_bot->GetInventoryRef().SetInventorySpace(inventory_size);

        for (const auto& item : items) {
            m_bot->GetInventoryRef().AddItem(item.ItemID, item.ItemAmount);
        }
    }
};