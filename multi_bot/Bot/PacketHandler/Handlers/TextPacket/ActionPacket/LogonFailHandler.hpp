#pragma once

#include "../../../IPacketHandler.hpp"

#include "../../../../Bot.h"

class LogonFailHandler : public IPacketHandler 
{
public:
    LogonFailHandler(Bot* bot_obj) : IPacketHandler::IPacketHandler(bot_obj) {}

    void Handle(const TextPacket* pkt) override {
        m_bot->GetLoggerPtr()->Error("Login Failed.");
        m_bot->Disconnect();
    }
};