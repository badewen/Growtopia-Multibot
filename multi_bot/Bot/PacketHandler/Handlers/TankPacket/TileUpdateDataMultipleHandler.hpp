#pragma once

#include "../../../Bot.h"
#include "../../IPacketHandler.hpp"

class TileUpdateDataMultipleHandler : public IPacketHandler
{
public:
    TileUpdateDataMultipleHandler(Bot* bot_obj) : IPacketHandler::IPacketHandler(bot_obj) {}

    void Handle(const TankPacket* tank_pkt) override {
        Utils::BinaryStreamReader reader{ tank_pkt->Value };

        uint32_t x = reader.GenericRead<uint32_t>();

        // marker if it ends
        while (x != 0xFFFFFFFF) {
            uint32_t y = reader.GenericRead<uint32_t>();

            WorldTile tile{};

            if (!WorldInfo::ParseTile(reader, tile, m_bot->GetItemDatabasePtr())) {
                m_bot->GetLoggerPtr()->Error("Unkown extra tile data type {} on block id {} is encountered in UPDATE_TILE_DATA_MULTIPLE Packet. Who knows what will happen now",
                    tile.ExtraTileDataType,
                    tile.Fg
                );
                return;
            }

            m_bot->GetCurrentWorldRef().SetTile(x, y, std::move(tile));
                
            x = reader.GenericRead<uint32_t>();
        }

    }
};