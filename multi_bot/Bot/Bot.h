#pragma once

#include <string>
#include <thread>
#include <memory>

// need to be here smh
#include "../Network/Http/HttpClient.h"

#include <enet/enet.h>
#include <proton/Variant.h>

#include "../Logger/ILogger.hpp"
#include "../Packet/PacketTypes.h"
#include "../Network/Enet/EnetClient.h"
#include "PacketHandler/PacketHandlerManager.h"
#include "NetAvatar.h"
#include "RedirectServerData.h"
#include "LoginData.h"

class Bot : public EnetClient
{
public:
    Bot(std::shared_ptr<ILogger> logger)
        : EnetClient::EnetClient{ logger },
        m_logger{ logger }
    {
        GenerateNewSpoof();
        m_default_packet_handler_registry = std::make_shared<PacketHandlerRegistry>( this );
        m_packet_handler_manager.SetHandlerRegistry(m_default_packet_handler_registry);
    }

        
    void __nothrow GenerateNewSpoof();

    void SetLoginGuest(std::string mac, std::string rid);
    void SetLoginGrowID(std::string growid, std::string password);

    bool ConnectWithHttp();
    bool Connect(std::string ip, std::string port, std::string meta, bool use_new_packet);

    void StartBotThread();
    void StopBotThread();

    void JoinWorld(std::string world);
    void Move(float x, float y);

    bool IsInGame() { return m_is_in_game; }

    void AlwaysReconnect(bool reconnect) { m_always_reconnect = reconnect; }

    std::unordered_map<int32_t, NetAvatar>* GetPlayerListPtr() { return &m_player_list; }
    NetAvatar* GetLocalPtr() { return &m_local; }
    std::shared_ptr<ILogger> GetLoggerPtr() { return m_logger; }
    RedirectServerData GetRedirectData() { return m_redirect_server_data; }
    LoginData GetLoginData() { return m_login_data; }

    void SetIsInWorld(bool in_world) { m_is_in_world = in_world; }
    void SetIsInGame(bool in_game) { m_is_in_game = in_game; }
    void SetIsRedirected(bool is_redirected) { m_is_redirected = is_redirected; }
    void SetShouldReconnect(bool should_reconnect) { m_reconnect = should_reconnect; }

    void SetRedirectData(const RedirectServerData* server_data);
    void SetServerIp(std::string ip) { m_server_ip = ip; }
    void SetServerPort(std::string port) { m_server_port = port; }

private:
    void bot_thread();

    void on_connect() override;
    void on_receive(ENetPacket* pkt) override;
    void on_disconnect() override;

    void on_incoming_text_packet(ePacketType type, TextPacket pkt);
    void on_incoming_tank_packet(TankPacket pkt);
    void on_incoming_varlist(VariantList varlist, TankPacket pkt);
private:
    LoginData m_login_data{};

    bool m_is_in_world = false, m_is_in_game = false;
    bool m_is_redirected = false, m_reconnect = false;

    bool m_is_running = false, m_using_new_packet = false, m_always_reconnect = false;

    std::string m_server_ip{}, m_server_port{};

    RedirectServerData m_redirect_server_data{};

    std::thread m_bot_thread{};

    NetAvatar m_local{};
    // netid, netavatar
    std::unordered_map<int32_t, NetAvatar> m_player_list{};

    bool m_is_bot_moving = false;

    PacketHandlerManager m_packet_handler_manager{};
    std::shared_ptr<PacketHandlerRegistry> m_default_packet_handler_registry{};

    std::shared_ptr<ILogger> m_logger{};
    HttpClient m_http_cl { "https://www.growtopia1.com" };
};

