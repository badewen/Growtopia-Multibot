#pragma once

#include "../../../Bot.h"
#include "../../IPacketHandler.hpp"
#include "../../../World/WorldInfo.h"

class SendMapDataHandler : public IPacketHandler
{
public:
    SendMapDataHandler(Bot* bot_obj) : IPacketHandler::IPacketHandler(bot_obj) {}

    void Handle(const TankPacket* tank_pkt) override {
        std::optional<WorldTile> faulty_tile{};
        if (!m_bot->GetCurrentWorldRef().SerializeFromMem(tank_pkt->Value, faulty_tile)) {
            m_bot->GetLoggerPtr()->Error("World Parsing Failed. Unknown Tile extra {} on block id {}",
                faulty_tile->Fg,
                faulty_tile->ExtraTileDataType
            );
        }
        else {
            m_bot->GetLoggerPtr()->Info("World Parsing Successful");
        }
    }
};