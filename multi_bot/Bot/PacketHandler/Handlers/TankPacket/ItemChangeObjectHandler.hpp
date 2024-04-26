#pragma once

#include "../../../Bot.h"
#include "../../IPacketHandler.hpp"
#include "../../../World/WorldItemObject.h"

class ItemChangeObjectHandler : public IPacketHandler
{
public:
    ItemChangeObjectHandler(Bot* bot_obj) : IPacketHandler::IPacketHandler(bot_obj) {}

    void Handle(const TankPacket* tank_pkt) override {
        if (!m_bot->GetCurrentWorldRef().IsParsedSuccessfully()) {
            return;
        }

        if (tank_pkt->Header.NetId == -1) {
            m_bot->GetCurrentWorldRef().AddItemObject(
                tank_pkt->Header.MainData,
                tank_pkt->Header.PositionX,
                tank_pkt->Header.PositionY,
                static_cast<uint32_t>(tank_pkt->Header.ObjectCount)
            );
            return;
        }
        else {
            WorldItemObject collected_obj = m_bot->GetCurrentWorldRef()
                                                  .GetItemObject(tank_pkt->Header.MainData);
            
            m_bot->GetCurrentWorldRef().RemoveItemObject(collected_obj.ObjectID);

            if (tank_pkt->Header.NetId == m_bot->GetLocalPtr()->NetID) {
                // 112 = gems id
                if (m_bot->GetItemDatabasePtr()->GetItemInfo(collected_obj.ItemID).ItemType == 
                    eItemInfoItemType::Gems
                ) {
                    m_bot->GetLocalPtr()->Gems += collected_obj.ItemAmount;
                }
                else {
                    m_bot->GetInventoryRef().AddItem(collected_obj.ItemID, collected_obj.ItemAmount);
                }
            }
        }
    }
};