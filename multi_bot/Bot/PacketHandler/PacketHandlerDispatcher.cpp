#include "PacketHandlerDispatcher.h"

#include "../../Utils/TextParse.h"

void PacketHandlerDispatcher::HandleHelloPacket() {
    std::shared_ptr<IPacketHandler> handler = m_handler_registry->GetHelloPacketHandler();

    if (handler) {
        handler->Handle();
    }
}

void PacketHandlerDispatcher::HandleGenericTextPacket(const TextPacket* pkt) {
    std::shared_ptr<IPacketHandler> handler = m_handler_registry->GetGenericTextPacketHandler();

    if (handler) {
        handler->Handle(pkt);
    }
}

void PacketHandlerDispatcher::HandleActionPacket(const TextPacket* pkt) {
    Utils::TextParse pkt_parse{ *pkt, "\n" };

    std::shared_ptr<IPacketHandler> handler = m_handler_registry->GetActionPacketHandler(pkt_parse.Get("action"));

    if (handler) {
        handler->Handle(pkt);
    }
}

void PacketHandlerDispatcher::HandleErrorPacket(const TextPacket* pkt) {
    std::shared_ptr<IPacketHandler> handler = m_handler_registry->GetErrorPacketHandler();

    if (handler) {
        handler->Handle(pkt);
    }
}

void PacketHandlerDispatcher::HandleTrackPacket(const TextPacket* pkt) {
    Utils::TextParse pkt_parse{ *pkt, "\n" };

    std::shared_ptr<IPacketHandler> handler = m_handler_registry->GetTrackPacketHandler(pkt_parse.Get("eventName"));

    if (handler) {
        handler->Handle(pkt);
    }
}

void PacketHandlerDispatcher::HandleLogRequestPacket(const TextPacket* pkt) {
    std::shared_ptr<IPacketHandler> handler = m_handler_registry->GetLogRequestPacketHandler();

    if (handler) {
        handler->Handle(pkt);
    }
}

void PacketHandlerDispatcher::HandleTankPacket(const TankPacket* pkt) {
    std::shared_ptr<IPacketHandler> handler = m_handler_registry->GetTankPacketHandler(pkt->Header.Type);

    if (handler) {
        handler->Handle(pkt);
    }
}

void PacketHandlerDispatcher::HandleVarlistPacket(const VariantList* varlist, const TankPacket* pkt) {
    std::shared_ptr<IPacketHandler> handler = m_handler_registry->GetVarlistPacketHandler(varlist->m_variant[0].GetString());

    if (handler) {
        handler->Handle(varlist, pkt);
    }
}
