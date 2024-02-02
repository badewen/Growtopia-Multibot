#pragma once

#include <string>
#include <thread>
#include <memory>

#include "../Network/Http/HttpClient.h"
#include <enet/enet.h>
#include <proton/Variant.h>

#include "../Logger/ILogger.h"
#include "../Packet/PacketTypes.h"
#include "../Network/Client/Client.h"

class Bot : public Client
{
public:
    Bot(std::shared_ptr<ILogger> logger)
        : Client::Client{ logger },
        m_logger{ logger }
    {
        GenerateNewSpoof();
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

private:
    void bot_thread();

    void on_connect() override;
    void on_receive(ENetPacket* pkt) override;
    void on_disconnect() override;

    void on_login();
    void on_login_fail();
    void on_redirect(VariantList* varlist);

    void on_incoming_packet(ePacketType type, TextPacket pkt);
    void on_incoming_tank_packet(TankPacket pkt);
    void on_incoming_varlist(VariantList varlist, TankPacket pkt);

private:
    std::string m_login_mac, m_login_rid;
    std::string m_login_wk, m_login_hash;

    std::string m_login_meta;
    std::string m_login_uuid_token;
    std::string m_login_door_id;
    std::string m_login_token;

    std::string m_login_growid, m_login_growid_pass;

    std::string m_game_version = "4.48";
    std::string m_game_proto_version = "203";

    bool m_is_in_world, m_is_running, m_is_in_game;
    bool m_is_redirected, m_reconnect, m_always_reconnect;
    bool m_using_new_packet;

    std::string m_server_ip, m_server_port;

    std::string m_user_id;

    std::thread m_bot_thread;

    float m_pos_x, m_pos_y;
    float m_last_pos_x, m_last_pos_y;
    bool m_is_bot_moving;
    

    std::shared_ptr<ILogger> m_logger;
    HttpClient m_http_cl { "https://www.growtopia1.com" };
};

