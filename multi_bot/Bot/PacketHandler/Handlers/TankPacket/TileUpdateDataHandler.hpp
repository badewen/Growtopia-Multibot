#pragma once

#include "../../../Bot.h"
#include "../../IPacketHandler.hpp"

class TileUpdateDataHandler : public IPacketHandler
{
public:
    TileUpdateDataHandler(Bot* bot_obj) : IPacketHandler::IPacketHandler(bot_obj) {}

    void Handle(const TankPacket* tank_pkt) override {
        Utils::BinaryStreamReader reader{ tank_pkt->Value };
        WorldTile parsed_tile{};

        bool parse_successful = WorldInfo::ParseTile(reader, parsed_tile);

        if (!parse_successful) {
            m_bot->GetLoggerPtr()->Error("Unkown extra tile data type {} on block id {} is encountered in UPDATE_TILE_DATA Packet. Who knows what will happen now",
                parsed_tile.ExtraTileDataType,
                parsed_tile.Fg
            );
            return;
        }

        m_bot->GetCurrentWorldRef().SetTile(
            tank_pkt->Header.TilePositionX,
            tank_pkt->Header.TilePositionY,
            std::move(parsed_tile)
        );

    }
};