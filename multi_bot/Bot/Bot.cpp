#include "Bot.h"

#include <spdlog/spdlog.h>

void Bot::GenerateNewSpoof() {
    auto gen_mac = [] () -> std::string {
        std::string ret = "";
        for (int i = 0; i < 6; i++) {
            
        }
    };
}

void Bot::SetLoginGuest(std::string mac, std::string rid) {
    m_login_mac = mac;
    m_login_rid = rid;

    m_login_growid = "";
    m_login_growid_pass = "";
}

void Bot::SetLoginGrowID(std::string growid, std::string password) {
    m_login_growid = growid;
    m_login_growid_pass = password;
}

bool Bot::CreateHost() {
    if (m_enet_host) {
        enet_host_destroy(m_enet_host);
        m_enet_host = nullptr;
    }

    m_enet_host = enet_host_create(nullptr, 1, 2, 0, 0);

    return m_enet_host;
}

bool Bot::Connect(std::string ip, std::string port, std::string meta, bool use_new_packet) {
    ENetAddress addr {};

    addr.host = std::stoi(port);
    enet_address_set_host_ip(&addr, ip.c_str());

    if (m_peer.GetRawPeer()) { m_peer.Disconnect(); }

    m_enet_host->usingNewPacket = use_new_packet;  

    m_peer = { enet_host_connect(m_enet_host, &addr, 2, 0, enet_host_random(m_enet_host)) };

    return m_peer.GetRawPeer();
}

void Bot::StartBotThread() {
    m_bot_thread = std::move(std::thread{ &Bot::bot_thread, this });
}

void Bot::Disconnect() {
    if (m_peer.GetRawPeer()) {
        m_peer.Disconnect();
    }
}

void Bot::bot_thread() {
    ENetEvent event{};
    while (m_peer.IsConnected()) {
        while (enet_host_service(m_enet_host, &event, 1) > 0) {
            switch (event.type)
            {
            case ENET_EVENT_TYPE_CONNECT:
                on_connect();
                break;
            case ENET_EVENT_TYPE_RECEIVE:
                on_receive(event.packet);
                break;
            case ENET_EVENT_TYPE_DISCONNECT:
                on_disconnect();
                break;
            default:
                break;
            }
        }
    }
}

void Bot::on_connect() {
    m_logger->Info("Bot with Id \"{}\" has connected to the server.", m_bot_id.empty() ? m_bot_id : "none");
}

void Bot::on_receive(ENetPacket* pkt) {

}

void Bot::on_disconnect() {
    m_logger->Info("Bot with Id \"{}\" has disconnected from the server.", m_bot_id.empty() ? m_bot_id : "none");
}
