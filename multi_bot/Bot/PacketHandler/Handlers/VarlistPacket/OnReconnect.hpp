#pragma once

#include <proton/Variant.h>

#include "../../../Bot.h"
#include "../../IPacketHandler.hpp"

class OnReconnectHandler : public IPacketHandler
{
public:
    OnReconnectHandler(Bot* bot_obj) : IPacketHandler::IPacketHandler(bot_obj) {}

    void Handle(const VariantList* varlist, const TankPacket* tank_pkt) override {
        m_bot->SetShouldReconnect(true);
        m_bot->Disconnect();
    }
};