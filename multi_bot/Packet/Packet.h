#pragma once

#include <stdint.h>
#include <vector>
#include <memory>
#include <string>

#include <enet/enet.h>

#include "PacketTypes.h"

class Packet
{
public:
    Packet(ENetPacket* enet_packet);
    Packet(ePacketType type, TextPacket text_pkt);
    Packet(TankPacket tank_pkt);
    Packet() = default;
    Packet(const Packet&) = delete;

    ~Packet();

    std::string ToDebugString();
    ENetPacket* CreateToENetPacket() const;
public:
    ePacketType Type;

    // if the type is any other than Tank Packet ype
    std::unique_ptr<TextPacket> TextPkt;

    // if the type is tank packet
    std::unique_ptr<TankPacket> TankPkt;
};

