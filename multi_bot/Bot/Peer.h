#pragma once

#include <enet/enet.h>

class Peer
{
public:
    Peer(ENetPeer* peer);

    inline ENetPeer* GetRawPeer() { return m_enet_peer; }

    inline bool IsConnected() { return m_enet_peer->state == ENET_PEER_STATE_CONNECTED; }

    inline void Disconnect() { 
        enet_peer_disconnect(m_enet_peer, NULL);
        m_enet_peer = nullptr;
    }

    inline void DisconnectNow() {
        enet_peer_disconnect_now(m_enet_peer, NULL);
        m_enet_peer = nullptr;
    }
private:

private:
    ENetPeer* m_enet_peer;

};

