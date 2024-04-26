#define ENET_IMPLEMENTATION

#include "EnetClient.h"
#include <chrono>

#include "../../Utils/TextParse.h"

using namespace std::chrono_literals;

bool EnetClient::CreateHost() {
    if (m_enet_host) {
        enet_host_destroy(m_enet_host);
        m_enet_host = nullptr;
    }

    m_enet_host = enet_host_create(nullptr, 1, 2, 0, 0);

    enet_host_compress_with_range_coder(m_enet_host);
    m_enet_host->checksum = enet_crc32;

    return m_enet_host;
}

bool EnetClient::Connect(std::string ip, std::string port, bool use_new_packet) {
    ENetAddress addr{};

    enet_address_set_host(&addr, ip.c_str());
    addr.port = std::stoi(port);

    if (m_peer.GetRawPeer()) { m_peer.Disconnect(); }

    m_enet_host->usingNewPacket = use_new_packet;

    m_logger->Info("Connecting to Growtopia server {}:{}", ip, addr.port);

    m_peer = { enet_host_connect(m_enet_host, &addr, 2, 0) };
    m_peer.SetLogger(m_logger);

    return m_peer.GetRawPeer();
}

void EnetClient::StartThread() {
    m_is_running = true;
    m_client_thread = std::move(std::thread{ &EnetClient::client_thread, this });
}

void EnetClient::StopThread() {
    m_is_running = false;
    m_client_thread.join();
}

void EnetClient::Disconnect() {
    if (m_peer.GetRawPeer()) {
        m_peer.Disconnect();
    }
}

void EnetClient::DisconnectNow() {
    if (m_peer.GetRawPeer()) {
        m_peer.DisconnectNow();
    }
}

void EnetClient::SetSocks5Url(std::string ip, std::string username, std::string pass) {
    m_enet_host->usingProxy = ip.empty();

    std::vector<std::string> ip_port = Utils::TextParse::StringTokenize(ip, ":");
    
    // since it takes char pointer, it will get invalidated if we simply used ip.c_str()
    m_proxy_info = {};
    m_proxy_info.ip = ip_port.at(0);
    m_proxy_info.port = ip_port.size() < 2 ? (uint16_t)1080 : std::stoi(ip_port.at(1));
    m_proxy_info.username = username;
    m_proxy_info.password = pass;

    m_enet_host->proxyInfo = {
        (char*)m_proxy_info.ip.c_str(),
        m_proxy_info.port,
        {
            (char*)m_proxy_info.username.c_str(),
            (char*)m_proxy_info.password.c_str()
        }
    };
}

void EnetClient::client_thread() {
    ENetEvent event{};
    while (m_is_running) {
        while (enet_host_service(m_enet_host, &event, 100) > 0) {
            switch (event.type)
            {
            case ENET_EVENT_TYPE_DISCONNECT:
                on_disconnect();
                break;
            case ENET_EVENT_TYPE_CONNECT:
                on_connect();
                break;
            case ENET_EVENT_TYPE_RECEIVE:
                on_receive(event.packet);
                break;
            default:
                break;
            }
        }
    }
}
