#pragma once

#include <string>
#include <thread>
#include <memory>

#include <enet/enet.h>

#include "../Logger/ILogger.h"
#include "Peer.h"

class Bot
{
public:
    Bot(std::unique_ptr<ILogger> logger)
        : m_logger{ std::move(logger) }
    {
        GenerateNewSpoof();
    }

    
    void __nothrow GenerateNewSpoof();

    // USED FOR LOGGING PURPOSE
    void SetBotID(std::string id) { m_bot_id = id; }

    void SetLoginGuest(std::string mac, std::string rid);
    void SetLoginGrowID(std::string growid, std::string password);

    bool CreateHost();

    bool Connect(std::string ip, std::string port, std::string meta, bool use_new_packet);

    void StartBotThread();

    void Disconnect();

private:
    void bot_thread();

    void on_connect();
    void on_receive(ENetPacket* pkt);
    void on_disconnect();

private:
    std::string m_login_mac, m_login_rid;
    std::string m_login_wk, m_login_hash, m_login_hash2;

    std::string m_login_meta;

    std::string m_login_growid, m_login_growid_pass;

    std::string m_game_version = "4.45";
    std::string m_game_proto_version = "203";
    
    uint32_t m_connect_id = 0;

    bool m_is_in_world;

    std::string m_server_ip, m_server_port;

    std::string m_bot_id;

    std::thread m_bot_thread;

    std::unique_ptr<ILogger> m_logger;
    ENetHost* m_enet_host;
    Peer m_peer {nullptr};
};

