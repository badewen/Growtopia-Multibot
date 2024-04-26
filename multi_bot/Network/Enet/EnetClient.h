#pragma once
#include <memory>
#include <thread>

#include <enet/enet.h>
#include "../../Logger/ILogger.hpp"
#include "../../Packet/Packet.h"

#include "EnetPeer.h"

class EnetClient
{
public:
    EnetClient(std::shared_ptr<ILogger> logger) :
        m_logger{ logger }
    {}

    bool CreateHost();

    bool Connect(std::string ip, std::string port, bool use_new_packet);

    void StartThread();
    void StopThread();

    void Disconnect();
    void DisconnectNow();

    bool IsConnected() { return m_peer.IsConnected(); }
    void SendPacket(const Packet* pkt) { m_peer.SendPacket(pkt); }
    void SendPacket(const Packet&& pkt) { m_peer.SendPacket(std::forward<const Packet&&>(pkt)); }
    void SendPacket(ENetPacket* pkt) { m_peer.SendPacket(pkt); }

    void SetSocks5Url(std::string ip, std::string username, std::string pass);

protected:
    virtual void on_connect() = 0;
    virtual void on_receive(ENetPacket* pkt) = 0;
    virtual void on_disconnect() = 0;

private:
    struct proxy_info
    {
        std::string ip;
        uint16_t port;
        std::string username;
        std::string password;
    };

private:
    void client_thread();

private:
    bool m_is_running = false;

    proxy_info m_proxy_info{};
    std::thread m_client_thread{};
    std::shared_ptr<ILogger> m_logger;
    ENetHost* m_enet_host{ nullptr };
    EnetPeer m_peer{ nullptr };
};

