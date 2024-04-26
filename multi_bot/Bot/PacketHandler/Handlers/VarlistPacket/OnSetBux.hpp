#pragma once

#include <proton/Variant.h>

#include "../../../Bot.h"
#include "../../../NetAvatar.h"
#include "../../IPacketHandler.hpp"

class OnSetBux : public IPacketHandler
{
public:
    OnSetBux(Bot* bot_obj) : IPacketHandler::IPacketHandler(bot_obj) {}

    void Handle(const VariantList* varlist, const TankPacket* tank_pkt) override {
        // [1] = gems amount
        m_bot->GetLocalPtr()->Gems = varlist->m_variant[1].GetINT32();
    }
};