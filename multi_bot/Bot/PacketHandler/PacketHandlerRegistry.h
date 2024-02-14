#pragma once

#include <memory>

#include <eventpp/eventdispatcher.h>

#include "IPacketHandler.hpp"

class Bot;

class PacketHandlerRegistry
{
public:
    PacketHandlerRegistry(Bot* bot_obj);

    std::shared_ptr<IPacketHandler> GetGenericTextPacketHandler();
    std::shared_ptr<IPacketHandler> GetLogonPacketHandler();
    std::shared_ptr<IPacketHandler> GetActionPacketHandler(std::string action_name);
    std::shared_ptr<IPacketHandler> GetTrackPacketHandler(std::string event_name);
    std::shared_ptr<IPacketHandler> GetErrorPacketHandler();
    std::shared_ptr<IPacketHandler> GetLogRequestPacketHandler();

    std::shared_ptr<IPacketHandler> GetTankPacketHandler(eTankPacketType tank_pkt_type);

    std::shared_ptr<IPacketHandler> GetVarlistPacketHandler(std::string varlist_func);
private:
    std::shared_ptr<IPacketHandler> m_generic_text_packet_handler{};
    std::shared_ptr<IPacketHandler> m_logon_packet_handler{};
    std::unordered_map<std::string /*action_name*/, std::shared_ptr<IPacketHandler>> m_action_packet_handlers{};
    std::unordered_map<std::string /*event_name*/, std::shared_ptr<IPacketHandler>> m_track_packet_handlers{};
    std::shared_ptr<IPacketHandler> m_error_packet_handler{};
    std::shared_ptr<IPacketHandler> m_log_request_packet_handler{};

    std::unordered_map<eTankPacketType, std::shared_ptr<IPacketHandler>> m_tank_packet_handlers{};

    std::unordered_map<std::string /*varlist_func*/, std::shared_ptr<IPacketHandler>> m_varlist_packet_handlers{};

    Bot* m_bot_ptr;
};

