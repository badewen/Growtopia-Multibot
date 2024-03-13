#pragma once

#include "../../../Bot.h"
#include "../../IPacketHandler.hpp"

class ModifyItemInventoryHandler : public IPacketHandler
{
public:
    ModifyItemInventoryHandler(Bot* bot_obj) : IPacketHandler::IPacketHandler(bot_obj) {}

    void Handle(const TankPacket* tank_pkt) override {
        m_bot->GetInventoryRef().RemoveItem(tank_pkt->Header.MainData, tank_pkt->Header.LostItemCount);
    }
};