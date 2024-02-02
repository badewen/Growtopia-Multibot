#pragma once
#include <memory>
#include <thread>

#include <enet/enet.h>
#include "../../Logger/ILogger.h"
#include "../../Packet/Packet.h"

#include "Peer.h"

class Client
{
public:
    Client(std::shared_ptr<ILogger> logger) :
        m_logger{ logger }
    {}

    bool CreateHost();

    bool Connect(std::string ip, std::string port, bool use_new_packet);

    void StartThread();
    void StopThread();

    void Disconnect();

    bool IsConnected() { return m_peer.IsConnected(); }
    void SendPacket(Packet* pkt) { m_peer.SendPacket(pkt); }
    void SendPacket(Packet&& pkt) { m_peer.SendPacket(std::forward<Packet&&>(pkt)); }
    void SendPacket(ENetPacket* pkt) { m_peer.SendPacket(pkt); }


protected:
    virtual void on_connect() = 0;
    virtual void on_receive(ENetPacket* pkt) = 0;
    virtual void on_disconnect() = 0;

private:
    void client_thread();

private:
    bool m_is_running;

    std::thread m_client_thread{};
    std::shared_ptr<ILogger> m_logger;
    ENetHost* m_enet_host;
    Peer m_peer{ nullptr };
};

