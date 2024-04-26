#pragma once

#include <memory>

#include <enet/enet.h>

#include "../../Logger/ILogger.hpp"
#include "../../Packet/Packet.h"

class EnetPeer
{
public:
    EnetPeer(ENetPeer* peer);

    inline ENetPeer* GetRawPeer() { return m_enet_peer; }

    inline bool IsConnected() { return m_enet_peer->state == ENET_PEER_STATE_CONNECTED; }

    inline void SetLogger(std::shared_ptr<ILogger> logger) { m_logger = logger; }

    inline void Disconnect() { 
        enet_peer_disconnect(m_enet_peer, NULL);
        m_enet_peer = nullptr;
    }

    inline void DisconnectNow() {
        enet_peer_disconnect_now(m_enet_peer, NULL);
        m_enet_peer = nullptr;
    }

    inline void SendPacket(ENetPacket* pkt) {
        if (m_enet_peer) {
            if (m_logger.get()) {
                m_logger->LogString(ILogger::LogType::Info, "Outgoing " + Packet{ pkt }.ToDebugString());
            }

            enet_peer_send(m_enet_peer, 0, pkt);
        }
    }

    void SendPacket(const Packet* pkt);
    inline void SendPacket(const Packet&& pkt) { 
        SendPacket(&pkt);
    }

private:

private:
    ENetPeer* m_enet_peer;
    // used for logging outgoing packets
    std::shared_ptr<ILogger> m_logger;
};

