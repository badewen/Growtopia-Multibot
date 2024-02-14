#pragma once

#include <memory>

#include <proton/Variant.h>

#include "../../Packet/PacketTypes.h"

class Bot;

class IPacketHandler 
{
public:
    IPacketHandler(Bot* bot_obj) :
        m_bot{ bot_obj }
    {
    }

    virtual void Handle() {}
    virtual void Handle(const TextPacket* pkt) {}
    virtual void Handle(const TankPacket* pkt) {}
    virtual void Handle(const VariantList* varlist, const TankPacket* pkt) {}

public:
    Bot* m_bot;
};