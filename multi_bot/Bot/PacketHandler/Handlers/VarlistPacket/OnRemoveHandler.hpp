#pragma once

#include <proton/Variant.h>

#include "../../../Bot.h"
#include "../../../NetAvatar.h"
#include "../../IPacketHandler.hpp"
#include "../../../../Utils/TextParse.h"

class OnRemoveHandler : public IPacketHandler
{
public:
    OnRemoveHandler(Bot* bot_obj) : IPacketHandler::IPacketHandler(bot_obj) {}

    void Handle(const VariantList* varlist, const TankPacket* tank_pkt) override {
        m_bot->GetPlayerListPtr()->erase(
            std::stoi(
                Utils::TextParse::StringTokenize(varlist->m_variant[1].GetString(), "|").at(1)
            )
        );
    }
};