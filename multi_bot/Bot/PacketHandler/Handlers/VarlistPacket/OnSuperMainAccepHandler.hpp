#pragma once

#include <proton/Variant.h>

#include "../../../Bot.h"
#include "../../IPacketHandler.hpp"

class OnSuperMainAcceptHandler : public IPacketHandler
{
public:
    OnSuperMainAcceptHandler(Bot* bot_obj) : IPacketHandler::IPacketHandler(bot_obj) {}

    void Handle(const VariantList* varlist, const TankPacket* tank_pkt) override {
        m_bot->SetIsInGame(true);
        m_bot->SendPacket({ ePacketType::NET_MESSAGE_GENERIC_TEXT, "action|enter_game" });
    }
};