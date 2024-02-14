#include "PacketHandlerManager.h"

#include "../../Utils/TextParse.h"

void PacketHandlerManager::HandleGenericTextPacket(const TextPacket* pkt) {
    std::shared_ptr<IPacketHandler> handler = m_handler_registry->GetGenericTextPacketHandler();

    if (handler) {
        handler->Handle(pkt);
    }
}

void PacketHandlerManager::HandleActionPacket(const TextPacket* pkt) {
    Utils::TextParse pkt_parse{ *pkt, "\n" };

    std::shared_ptr<IPacketHandler> handler = m_handler_registry->GetActionPacketHandler(pkt_parse.Get("action"));

    if (handler) {
        handler->Handle(pkt);
    }
}

void PacketHandlerManager::HandleErrorPacket(const TextPacket* pkt) {
    std::shared_ptr<IPacketHandler> handler = m_handler_registry->GetErrorPacketHandler();

    if (handler) {
        handler->Handle(pkt);
    }
}

void PacketHandlerManager::HandleTrackPacket(const TextPacket* pkt) {
    Utils::TextParse pkt_parse{ *pkt, "\n" };

    std::shared_ptr<IPacketHandler> handler = m_handler_registry->GetTrackPacketHandler(pkt_parse.Get("eventName"));

    if (handler) {
        handler->Handle(pkt);
    }
}

void PacketHandlerManager::HandleLogRequestPacket(const TextPacket* pkt) {
    std::shared_ptr<IPacketHandler> handler = m_handler_registry->GetLogRequestPacketHandler();

    if (handler) {
        handler->Handle(pkt);
    }
}

void PacketHandlerManager::HandleTankPacket(const TankPacket* pkt) {
    std::shared_ptr<IPacketHandler> handler = m_handler_registry->GetTankPacketHandler(pkt->Header.Type);

    if (handler) {
        handler->Handle(pkt);
    }
}

void PacketHandlerManager::HandleVarlistPacket(const VariantList* varlist, const TankPacket* pkt) {
    std::shared_ptr<IPacketHandler> handler = m_handler_registry->GetVarlistPacketHandler(varlist->m_variant[0].GetString());

    if (handler) {
        handler->Handle(varlist, pkt);
    }
}
