#pragma once

#include "../../../../Bot.h"
#include "../../../IPacketHandler.hpp"
#include "../../../../../Utils/TextParse.h"

class WorldVisitHandler : public IPacketHandler
{
public:
    WorldVisitHandler(Bot* bot_obj) : IPacketHandler::IPacketHandler(bot_obj) {}

    void Handle(const TextPacket* pkt) override {
        Utils::TextParse pkt_parse{ *pkt, "\n" };

        m_bot->GetLocalPtr()->IsInvis = false;
        m_bot->GetLocalPtr()->NetID = 0;
        m_bot->GetLocalPtr()->FacingLeft = false;
        m_bot->GetLocalPtr()->LastPosX = 0;
        m_bot->GetLocalPtr()->LastPosY = 0;
        m_bot->GetLocalPtr()->PosX = 0;
        m_bot->GetLocalPtr()->PosY = 0;
        m_bot->GetLocalPtr()->WorldName = pkt_parse.Get("World_name").substr(2);

        m_bot->GetPlayerListPtr()->clear();
        m_bot->GetCurrentWorldRef().Reset();
    }
};