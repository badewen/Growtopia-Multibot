#pragma once

#include "../../../Bot.h"
#include "../../IPacketHandler.hpp"
#include "../../../../Packet/PacketTypes.h"

class PingRequestHandler : public IPacketHandler
{
public:
    PingRequestHandler(Bot* bot_obj) : IPacketHandler::IPacketHandler(bot_obj) {}

    void Handle(const TankPacket* pkt) override {
        TankPacket tank_pkt = {};

        //https://github.com/ama7nen/enetproxy/blob/62079ce101a0794dbe958790873533cb71b4fb08/proxy/events.cpp#L18
        tank_pkt.Header.Type = eTankPacketType::NET_GAME_PACKET_PING_REPLY;
        tank_pkt.Header.VectorX2 = 1000.f; // gravity
        tank_pkt.Header.VectorY2 = 250.f; // move speed
        tank_pkt.Header.VectorX = 64.f; // punch range
        tank_pkt.Header.VectorY = 64.f; // build range
        tank_pkt.Header.JumpCount = 0;

        m_bot->SendPacket({ tank_pkt });
    }
};