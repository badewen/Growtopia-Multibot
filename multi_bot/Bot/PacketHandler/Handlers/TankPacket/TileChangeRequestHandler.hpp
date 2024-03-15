#pragma once

#include "../../../Bot.h"
#include "../../IPacketHandler.hpp"

class TileChangeRequestHandler : public IPacketHandler
{
public:
    TileChangeRequestHandler(Bot* bot_obj) : IPacketHandler::IPacketHandler(bot_obj) {}

    void Handle(const TankPacket* tank_pkt) override {
        if (!m_bot->GetCurrentWorldRef().IsParsedSuccessfully()) {
            return;
        }

        const WorldTile& old_tile =  m_bot->GetCurrentWorldRef()
                                           .GetTileRef(
                                                tank_pkt->Header.TilePositionX,
                                                tank_pkt->Header.TilePositionY
                                           );

        WorldTile new_tile{};

        // foreground always breaks first
        if (tank_pkt->Header.MainData == 18) {
            if (old_tile.Fg == 0) {
                new_tile.Bg = 0;
            }
            else {
                new_tile.Fg = 0;
            }

            new_tile.ExtraTileDataType = eWorldTileExtraType::None;
        }
        // place block
        else {
            auto block_info = m_bot->GetItemDatabasePtr()->GetItemInfo(tank_pkt->Header.MainData);
            if (block_info.ItemType == eItemInfoItemType::Background) {
                new_tile.Bg = block_info.ItemID;
            }
            else {
                new_tile.Fg = block_info.ItemID;
            }

            if (tank_pkt->Header.NetId == m_bot->GetLocalPtr()->NetID) {
                m_bot->GetInventoryRef().RemoveItem(tank_pkt->Header.MainData, 1);
            }
        }
        new_tile.Flags.bWater = old_tile.Flags.bWater;
        new_tile.Flags.bLocked = old_tile.Flags.bLocked;

        new_tile.LockIndex = old_tile.LockIndex;
        new_tile.ParentTileIndex = old_tile.ParentTileIndex;

        m_bot->GetCurrentWorldRef().SetTile(
            tank_pkt->Header.TilePositionX,
            tank_pkt->Header.TilePositionY,
            new_tile
        );
    }
};