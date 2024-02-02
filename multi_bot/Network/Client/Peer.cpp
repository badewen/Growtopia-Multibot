#include "Peer.h"

Peer::Peer(ENetPeer* peer) {
    m_enet_peer = peer;
}

void Peer::SendPacket(Packet* pkt) {
    if (!pkt) {
        return;
    }

    SendPacket(pkt->CreateToENetPacket());
}