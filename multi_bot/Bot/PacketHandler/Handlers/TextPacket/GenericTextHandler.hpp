#pragma once

#include "../../IPacketHandler.hpp"
#include "../../../Bot.h"

class GenericTextHandler : public IPacketHandler
{
public:
    GenericTextHandler(Bot* bot_obj) : IPacketHandler::IPacketHandler(bot_obj) {}

    void Handle(const TextPacket* pkt) override {
        m_bot->GetLoggerPtr()->Debug("GENERIC TEXT WAS SENT BY THE SERVER !!!!");
    }
};