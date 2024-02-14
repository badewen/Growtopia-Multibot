#pragma once

#include <proton/Variant.h>

#include "../../../Bot.h"
#include "../../IPacketHandler.hpp"

class OnSetPosHandler : public IPacketHandler
{
public:
    OnSetPosHandler(Bot* bot_obj) : IPacketHandler::IPacketHandler(bot_obj) {}
    
    void Handle(const VariantList* varlist, const TankPacket* tank_pkt) override {
        CL_Vec2f pos = varlist->m_variant[1].GetVector2();
        m_bot->GetLocalPtr()->PosX = pos.x;
        m_bot->GetLocalPtr()->PosY = pos.y;
    }
};