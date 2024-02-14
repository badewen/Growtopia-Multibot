#pragma once

#include <proton/Variant.h>

#include "../../../Bot.h"
#include "../../IPacketHandler.hpp"

class PacketStateHandler : public IPacketHandler
{
public:
    PacketStateHandler(Bot* bot_obj) : IPacketHandler::IPacketHandler(bot_obj) {}

    void Handle(const TankPacket* tank_pkt) override {
        if (m_bot->GetPlayerListPtr()->find(tank_pkt->Header.NetId) != m_bot->GetPlayerListPtr()->end()) {
            NetAvatar& current_netavatar = m_bot->GetPlayerListPtr()->at(tank_pkt->Header.NetId);
            current_netavatar.LastPosX = current_netavatar.PosX;
            current_netavatar.LastPosY = current_netavatar.PosY;

            current_netavatar.PosX = tank_pkt->Header.PositionX;
            current_netavatar.PosY = tank_pkt->Header.PositionY;
            current_netavatar.Flags = tank_pkt->Header.Flags;
        }
    }
};