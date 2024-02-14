#pragma once

#include "../../../IPacketHandler.hpp"

#include "../../../../Bot.h"

class LogonPacketHandler : public IPacketHandler
{
public:
    LogonPacketHandler(Bot* bot_obj) : IPacketHandler::IPacketHandler(bot_obj) {}

    void Handle();
};