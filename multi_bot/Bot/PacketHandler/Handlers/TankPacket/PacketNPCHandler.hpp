#pragma once

#include "../../../Bot.h"
#include "../../IPacketHandler.hpp"

#include "../../../World/WorldNPC.h"
#include "../../../../Utils/BinaryStreamReader.h"

class PacketNPCHandler : public IPacketHandler
{
public:
#pragma pack(push, 1)
    struct RawNPCPkt
    {
        uint8_t NpcType, NpcIndex;
        float PosX, PosY, PosX2, PosY2;
        float Unk1, Unk2;
        float Speed;
    };
#pragma pack(pop)
public:
    PacketNPCHandler(Bot* bot_obj) : IPacketHandler::IPacketHandler(bot_obj) {}

    void Handle(const TankPacket* tank_pkt) override {
        if (tank_pkt->Header.DataLength != 0) {
            Utils::BinaryStreamReader reader{ tank_pkt->Value };
            for (const auto& npc : reader.ReadList<uint8_t, RawNPCPkt>()) {
                WorldNPC npc_data{};
                npc_data.Index = npc.NpcIndex;
                npc_data.PosX = npc.PosX;
                npc_data.PosY = npc.PosY;
                npc_data.TargetX = npc.PosX2;
                npc_data.TargetY = npc.PosY2;
                npc_data.Type = (eWorldNPCType)npc.NpcType;
                npc_data.Speed = npc.Speed;
                
                m_bot->GetCurrentWorldRef().SetNPC(npc_data);
            }
            return;
        }

        if (tank_pkt->Header.NpcAction == eTankPacketNPCActionType::Add) {
            WorldNPC npc_data{};
            npc_data.Index = tank_pkt->Header.NpcIndex;
            npc_data.PosX = tank_pkt->Header.PositionX;
            npc_data.PosY = tank_pkt->Header.PositionY;
            npc_data.TargetX = tank_pkt->Header.PositionX2;
            npc_data.TargetY = tank_pkt->Header.PositionY2;
            npc_data.Type = (eWorldNPCType)tank_pkt->Header.NpcType;
            npc_data.Speed = tank_pkt->Header.NpcVariable;
            
            m_bot->GetCurrentWorldRef().SetNPC(npc_data);
        }
        else if (tank_pkt->Header.NpcAction == eTankPacketNPCActionType::MoveTo) {
            WorldNPC npc_data{};
            npc_data.Index = tank_pkt->Header.NpcIndex;
            npc_data.PosX = tank_pkt->Header.PositionX;
            npc_data.PosY = tank_pkt->Header.PositionY;
            npc_data.TargetX = tank_pkt->Header.PositionX2;
            npc_data.TargetY = tank_pkt->Header.PositionY2;
            npc_data.Type = (eWorldNPCType)tank_pkt->Header.NpcType;
            npc_data.Speed = tank_pkt->Header.NpcVariable;
            
            m_bot->GetCurrentWorldRef().SetNPC(npc_data);
        }
        else if (
            tank_pkt->Header.NpcAction == eTankPacketNPCActionType::Sucked ||
            tank_pkt->Header.NpcAction == eTankPacketNPCActionType::Die ||
            tank_pkt->Header.NpcAction == eTankPacketNPCActionType::Delete
        ) {
            m_bot->GetCurrentWorldRef().RemoveNPC(tank_pkt->Header.NpcIndex);
        }
    }
};