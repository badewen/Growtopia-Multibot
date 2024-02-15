#pragma once

#include <proton/Variant.h>

#include "../../Packet/Packet.h"
#include "PacketHandlerRegistry.h"

class PacketHandlerManager
{
public:
    void HandleHelloPacket();
    void HandleGenericTextPacket(const TextPacket* pkt);
    void HandleActionPacket(const TextPacket* pkt);
    void HandleErrorPacket(const TextPacket* pkt);
    void HandleTrackPacket(const TextPacket* pkt);
    void HandleLogRequestPacket(const TextPacket* pkt);

    void HandleTankPacket(const TankPacket* pkt);

    void HandleVarlistPacket(const VariantList* varlist, const TankPacket* pkt);

    // MUST BE CALLED BEFORE DOING ANYTHING!!!
    void SetHandlerRegistry(std::shared_ptr<PacketHandlerRegistry> reg) { m_handler_registry = reg; }

private:
    std::shared_ptr<PacketHandlerRegistry> m_handler_registry;
};

