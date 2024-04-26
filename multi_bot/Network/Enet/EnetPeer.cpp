#include "EnetPeer.h"

EnetPeer::EnetPeer(ENetPeer* peer) {
    m_enet_peer = peer;
}

void EnetPeer::SendPacket(const Packet* pkt) {
    if (!pkt) {
        return;
    }

    SendPacket(pkt->CreateToENetPacket());
}