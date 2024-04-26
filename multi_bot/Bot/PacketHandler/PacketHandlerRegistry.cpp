#include "PacketHandlerRegistry.h"

#include "Handlers/TankPacket/PacketStateHandler.hpp"
#include "Handlers/TankPacket/PingRequestHandler.hpp"
#include "Handlers/TankPacket/InventoryStateHandler.hpp"
#include "Handlers/TankPacket/SendMapDataHandler.hpp"
#include "Handlers/TankPacket/ItemChangeObjectHandler.hpp"
#include "Handlers/TankPacket/ModifyItemInventoryHandler.hpp"
#include "Handlers/TankPacket/TileChangeRequestHandler.hpp"
#include "Handlers/TankPacket/TileUpdateDataHandler.hpp"
#include "Handlers/TankPacket/PacketNPCHandler.hpp"

#include "Handlers/TextPacket/ActionPacket/LogonFailHandler.hpp"
#include "Handlers/TextPacket/HelloPacket/HelloPacketHandler.hpp"

#include "Handlers/TextPacket/TrackPacket/WorldVisitHandler.hpp"
#include "Handlers/TextPacket/TrackPacket/WorldExitHandler.hpp"

#include "Handlers/TextPacket/GenericTextHandler.hpp"

#include "Handlers/VarlistPacket/OnReconnect.hpp"
#include "Handlers/VarlistPacket/OnRemoveHandler.hpp"
#include "Handlers/VarlistPacket/OnSendToServerHandler.hpp"
#include "Handlers/VarlistPacket/OnSetPosHandler.hpp"
#include "Handlers/VarlistPacket/OnSpawnHandler.hpp"
#include "Handlers/VarlistPacket/OnSuperMainAccepHandler.hpp"

PacketHandlerRegistry::PacketHandlerRegistry(Bot* bot_obj) {
    m_bot_ptr = bot_obj;

    // Tank Packets
    m_tank_packet_handlers.emplace(eTankPacketType::NET_GAME_PACKET_STATE, std::make_shared<PacketStateHandler>(bot_obj));
    m_tank_packet_handlers.emplace(eTankPacketType::NET_GAME_PACKET_PING_REQUEST, std::make_shared<PingRequestHandler>(bot_obj));
    m_tank_packet_handlers.emplace(eTankPacketType::NET_GAME_PACKET_SEND_INVENTORY_STATE, std::make_shared<InventoryStateHandler>(bot_obj));
    m_tank_packet_handlers.emplace(eTankPacketType::NET_GAME_PACKET_SEND_MAP_DATA, std::make_shared<SendMapDataHandler>(bot_obj));
    m_tank_packet_handlers.emplace(eTankPacketType::NET_GAME_PACKET_ITEM_CHANGE_OBJECT, std::make_shared<ItemChangeObjectHandler>(bot_obj));
    m_tank_packet_handlers.emplace(eTankPacketType::NET_GAME_PACKET_MODIFY_ITEM_INVENTORY, std::make_shared<ModifyItemInventoryHandler>(bot_obj));
    m_tank_packet_handlers.emplace(eTankPacketType::NET_GAME_PACKET_TILE_CHANGE_REQUEST, std::make_shared<TileChangeRequestHandler>(bot_obj));
    m_tank_packet_handlers.emplace(eTankPacketType::NET_GAME_PACKET_SEND_TILE_UPDATE_DATA, std::make_shared<TileUpdateDataHandler>(bot_obj));
    m_tank_packet_handlers.emplace(eTankPacketType::NET_GAME_PACKET_NPC, std::make_shared<PacketNPCHandler>(bot_obj));

    // Action Packets
    m_action_packet_handlers.emplace("logon_fail", std::make_shared<LogonFailHandler>(bot_obj));

    // Track Packets
    m_track_packet_handlers.emplace("300_WORLD_VISIT", std::make_shared<WorldVisitHandler>(bot_obj));
    m_track_packet_handlers.emplace("worldexit", std::make_shared<WorldExitHandler>(bot_obj));
    
    // Hello Packet
    m_hello_packet_handler = std::make_shared<HelloPacketHandler>(bot_obj);

    // Generic Text
    m_generic_text_packet_handler = std::make_shared<GenericTextHandler>(bot_obj);

    // Variant list
    m_varlist_packet_handlers.emplace("OnReconnect", std::make_shared<OnReconnectHandler>(bot_obj));
    m_varlist_packet_handlers.emplace("OnRemove", std::make_shared<OnRemoveHandler>(bot_obj));
    m_varlist_packet_handlers.emplace("OnSendToServer", std::make_shared<OnSendToServerHandler>(bot_obj));
    m_varlist_packet_handlers.emplace("OnSetPos", std::make_shared<OnSetPosHandler>(bot_obj));
    m_varlist_packet_handlers.emplace("OnSpawn", std::make_shared<OnSpawnHandler>(bot_obj));
    m_varlist_packet_handlers.emplace("OnSuperMainStartAcceptLogonHrdxs47254722215a", std::make_shared<OnSuperMainAcceptHandler>(bot_obj));

}

std::shared_ptr<IPacketHandler> PacketHandlerRegistry::GetHelloPacketHandler() {
    return m_hello_packet_handler;
}

std::shared_ptr<IPacketHandler> PacketHandlerRegistry::GetGenericTextPacketHandler() {
    return m_generic_text_packet_handler;
}

std::shared_ptr<IPacketHandler> PacketHandlerRegistry::GetActionPacketHandler(std::string action_name) {
    auto found = m_action_packet_handlers.find(action_name);
     
    if (found != m_action_packet_handlers.end()) {
        return found->second;
    }

    return nullptr;
}

std::shared_ptr<IPacketHandler> PacketHandlerRegistry::GetTrackPacketHandler(std::string event_name) {
    auto found = m_track_packet_handlers.find(event_name);

    if (found != m_track_packet_handlers.end()) {
        return found->second;
    }

    return nullptr;
}

std::shared_ptr<IPacketHandler> PacketHandlerRegistry::GetErrorPacketHandler() {
    return m_error_packet_handler;
}

std::shared_ptr<IPacketHandler> PacketHandlerRegistry::GetLogRequestPacketHandler() {
    return m_log_request_packet_handler;
}

std::shared_ptr<IPacketHandler> PacketHandlerRegistry::GetTankPacketHandler(eTankPacketType tank_pkt_type) {
    auto found = m_tank_packet_handlers.find(tank_pkt_type);

    if (found != m_tank_packet_handlers.end()) {
        return found->second;
    }

    return nullptr;
}

std::shared_ptr<IPacketHandler> PacketHandlerRegistry::GetVarlistPacketHandler(std::string varlist_func) {
    auto found = m_varlist_packet_handlers.find(varlist_func);

    if (found != m_varlist_packet_handlers.end()) {
        return found->second;
    }

    return nullptr;
}
