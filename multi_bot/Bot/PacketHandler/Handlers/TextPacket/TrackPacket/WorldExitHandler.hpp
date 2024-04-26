#pragma once

#include "../../../../Bot.h"
#include "../../../IPacketHandler.hpp"
#include "../../../../../Utils/TextParse.h"

class WorldExitHandler : public IPacketHandler
{
public:
    WorldExitHandler(Bot* bot_obj) : IPacketHandler::IPacketHandler(bot_obj) {}

    void Handle(const TextPacket* pkt) override {
        Utils::TextParse pkt_parse{ *pkt, "\n" };

        m_bot->SetIsInWorld(false);

        *m_bot->GetLocalPtr() = {};
        m_bot->GetLocalPtr()->WorldName = "EXIT";
        m_bot->GetPlayerListPtr()->clear();
        m_bot->GetCurrentWorldRef().Reset();

    }
};