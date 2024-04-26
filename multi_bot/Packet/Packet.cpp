#include "Packet.h"

#include <magic_enum/magic_enum.hpp>
#include <proton/Variant.h>
#include <fmt/fmt.h>
#include <fmt/bin_to_hex.h>

#include "../Utils/TextParse.h"

Packet::Packet(ENetPacket* enet_packet) {
    if (enet_packet->dataLength < 5) {
        return;
    }

    Type = *(ePacketType*)enet_packet->data;

    if (Type != ePacketType::NET_MESSAGE_GAME_PACKET) {
        TextPkt = std::make_unique<std::string>( 
            enet_packet->data + sizeof(ePacketType),
            enet_packet->data + enet_packet->dataLength - 1 
        );
    }
    else {
        TankPkt = std::make_unique<TankPacket>();

        memcpy(&TankPkt->Header, enet_packet->data + sizeof(ePacketType), sizeof(TankPacket::Header));

        if (TankPkt->Header.DataLength > 0 || TankPkt->Header.Flags.bExtended) {
            TankPkt->Value.resize(TankPkt->Header.DataLength);

            memcpy(
                TankPkt->Value.data(),
                enet_packet->data + sizeof(ePacketType) + sizeof(TankPacket::Header),
                TankPkt->Header.DataLength
            );
        }
    }
    return;
}

Packet::Packet(ePacketType type, TextPacket text_pkt) {
    Type = type;

    TextPkt = std::make_unique<TextPacket>(text_pkt);
}

Packet::Packet(TankPacket tank_pkt) {
    Type = ePacketType::NET_MESSAGE_GAME_PACKET;

    TankPkt = std::make_unique<TankPacket>(tank_pkt);
}

Packet::~Packet() {
    if (Type == ePacketType::NET_MESSAGE_GAME_PACKET) {
        TankPkt.release();
    }
    else {
        TextPkt.release();
    }
    
    Type = ePacketType::NET_MESSAGE_UNKNOWN;
}

std::string Packet::ToDebugString() {
    if (Type == ePacketType::NET_MESSAGE_GAME_PACKET) {
        if (TankPkt->Header.Type == eTankPacketType::NET_GAME_PACKET_CALL_FUNCTION) {
            VariantList varlist {};
            varlist.SerializeFromMem(TankPkt->Value.data(), static_cast<int>(TankPkt->Value.size()));

            return fmt::format("VariantList, netid: {} \n{}\n", TankPkt->Header.NetId, varlist.GetContentsAsDebugString());
        }

        if (!TankPkt->Header.Flags.bExtended || 
            TankPkt->Header.Type == eTankPacketType::NET_GAME_PACKET_SEND_MAP_DATA ||
            TankPkt->Header.Type == eTankPacketType::NET_GAME_PACKET_SEND_ITEM_DATABASE_DATA    
        ) {
            return fmt::format(
                "TankPacket, netid: {}, type {}[{}]\n",
                TankPkt->Header.NetId,
                magic_enum::enum_name<eTankPacketType>(TankPkt->Header.Type),
                TankPkt->Header.Type
            );
        }
        else {
            return fmt::format(
                "TankPacket, netid: {} \n{}[{}] with extended data : \n{}\n",
                TankPkt->Header.NetId,
                magic_enum::enum_name<eTankPacketType>(TankPkt->Header.Type),
                TankPkt->Header.Type,
                spdlog::to_hex(TankPkt->Value.begin(), TankPkt->Value.end())
            );
        }
    }

    return fmt::format(
        "TextPacket: \n{}[{}]: \n{}\n",
        magic_enum::enum_name<ePacketType>(Type),
        Type,
        fmt::join(Utils::TextParse::StringTokenizeFormatted(*TextPkt, "\n"), "\n")
    );
}

ENetPacket* Packet::CreateToENetPacket() const {
    if (Type == ePacketType::NET_MESSAGE_GAME_PACKET) {
        ENetPacket* pkt = enet_packet_create(
            nullptr,
            sizeof(ePacketType) + sizeof(TankPacket::Header) + TankPkt->Header.DataLength,
            ENET_PACKET_FLAG_RELIABLE
        );

        *(ePacketType*)pkt->data = ePacketType::NET_MESSAGE_GAME_PACKET;
        memcpy(pkt->data + sizeof(ePacketType), &TankPkt->Header, sizeof(TankPacket::Header));
        memcpy(pkt->data + sizeof(ePacketType) + sizeof(TankPacket::Header), TankPkt->Value.data(), TankPkt->Value.size());

        return pkt;
    }
    else {
        ENetPacket* pkt = enet_packet_create(
            nullptr,
            sizeof(ePacketType) + TextPkt->length() + 1, // null terminator
            ENET_PACKET_FLAG_RELIABLE
        );

        *(ePacketType*)pkt->data = Type;
        memcpy(pkt->data + sizeof(ePacketType), TextPkt->data(), TextPkt->length());

        return pkt;
    }

    return nullptr;
}
