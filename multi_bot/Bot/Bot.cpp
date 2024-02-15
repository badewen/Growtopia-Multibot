#define _SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS

#include "Bot.h"

#include <chrono>

#include <proton/Variant.h>
#include <magic_enum/magic_enum.hpp>

#include "../Utils/Random.h"
#include "../Utils/TextParse.h"
#include "../Utils/Hash.hpp"
#include "../Packet/Packet.h"

using namespace std::chrono_literals;

void Bot::GenerateNewSpoof() {
    m_login_data.Mac = Utils::Random::RandomMac();
    m_login_data.Rid = Utils::Random::RandomHex(32, true);
    m_login_data.Wk = Utils::Random::RandomHex(32, true);
    m_login_data.Hash = std::to_string(Utils::Hash::proton(fmt::format("{}RT", Utils::Random::RandomHex(16, true)).c_str()));
}

void Bot::SetLoginGuest(std::string mac, std::string rid) {
    m_login_data.Mac = mac;
    m_login_data.Rid = rid;

    m_login_data.GrowID = "";
    m_login_data.GrowIDPass = "";
}

void Bot::SetLoginGrowID(std::string growid, std::string password) {
    m_login_data.GrowID = growid;
    m_login_data.GrowIDPass = password;
}

bool Bot::ConnectWithHttp() {
retry:

    m_logger->Info("Getting server data...");
    httplib::Result res = m_http_cl.Post("/growtopia/server_data.php", {}, {}, "application/x-www-form-urlencoded");

    if (res.error() != httplib::Error::Success) {
        m_logger->Error("Failed to get server data. Retrying in 5 seconds. Err : {}",
            magic_enum::enum_name<httplib::Error>(res.error())
        );
        std::this_thread::sleep_for(5000ms);
        goto retry;
    }

    Utils::TextParse parse{ res->body, "\n" };

    if (!Connect(parse.Get("server"), parse.Get("port"), parse.Get("meta"), true)) {
        m_logger->Error("Failed to connect to server. Reconnecting to server in 3 seconds");
        goto retry;
    }

    return true;
}

bool Bot::Connect(std::string ip, std::string port, std::string meta, bool use_new_packet) { 
    m_login_data.Meta = meta;
    m_using_new_packet = use_new_packet;

    return EnetClient::Connect(ip, port, use_new_packet);
}

void Bot::StartBotThread() {
    m_is_running = true;
    EnetClient::StartThread();
    m_bot_thread = std::move(std::thread{ &Bot::bot_thread, this });
}

void Bot::StopBotThread() {
    m_is_running = false; 
    EnetClient::StopThread();
    m_bot_thread.join();
}

void Bot::JoinWorld(std::string world)
{
    SendPacket({
        ePacketType::NET_MESSAGE_GAME_MESSAGE,
        fmt::format("action|join_request\nname|{}\ninvitedWorld|0", world)
    });
}

void Bot::Move(float x, float y) {
    m_local.PosX += x * 32 /* the block width in pixel */ ;
    m_local.PosY += y * 32 /* the block height in pixel */ ;
}

void Bot::SetRedirectData(const RedirectServerData* server_data) {
    if (!server_data->Token.empty()) {
        m_redirect_server_data.Token = server_data->Token;
    }
    if (!server_data->UUIDToken.empty()) {
        m_redirect_server_data.UUIDToken = server_data->UUIDToken;
    }

    m_redirect_server_data.DoorID = server_data->DoorID;
    m_redirect_server_data.UserID = server_data->UserID;
}

void Bot::bot_thread() {
    m_logger->Debug("STARTED BOT THREAD");

    ENetEvent event{};
    while (m_is_running) {
        if (m_local.LastPosX == m_local.PosX && m_local.LastPosY == m_local.PosY && m_is_bot_moving) {
            TankPacket tank_pkt{};

            tank_pkt.Header.Type = eTankPacketType::NET_GAME_PACKET_STATE;
            tank_pkt.Header.IntX = -1;
            tank_pkt.Header.IntY = -1;
            tank_pkt.Header.VectorX = m_local.PosX;
            tank_pkt.Header.VectorY = m_local.PosY;
            tank_pkt.Header.VectorX2 = 0;
            tank_pkt.Header.VectorY2 = 0;
            tank_pkt.Header.Flags.bUnk = true;
            tank_pkt.Header.Flags.bOnSolid = true;

            SendPacket(tank_pkt);
            m_is_bot_moving = false;
        }
        if (m_local.LastPosX != m_local.PosX || m_local.LastPosY != m_local.PosY) {
            TankPacket tank_pkt{};

            tank_pkt.Header.Type = eTankPacketType::NET_GAME_PACKET_STATE;
            tank_pkt.Header.VectorX = m_local.PosX;
            tank_pkt.Header.VectorY = m_local.PosY;
            tank_pkt.Header.Flags.bUnk = true; // idk why but this is present on alot of packets
            tank_pkt.Header.IntX = -1;
            tank_pkt.Header.IntY = -1;

            if (m_local.PosX - m_local.LastPosX < 0) {
                tank_pkt.Header.VectorX2 = -250;
                tank_pkt.Header.Flags.bRotateLeft = true;
            }
            else if (m_local.PosX - m_local.LastPosX > 0) {
                tank_pkt.Header.VectorX2 = 250;
                tank_pkt.Header.Flags.bRotateLeft = false;
            }

            if (m_local.PosY - m_local.LastPosY < 0) {
                tank_pkt.Header.VectorY2 = -250;
            }
            else if (m_local.PosY - m_local.LastPosY > 0) {
                tank_pkt.Header.VectorY2 = 250;
            }

            tank_pkt.Header.Flags.bOnSolid = true;

            SendPacket(tank_pkt);

            m_local.Flags.bRotateLeft = tank_pkt.Header.Flags.bRotateLeft;
            m_local.LastPosX = m_local.PosX;
            m_local.LastPosY = m_local.PosY;
            m_is_bot_moving = true;
        }
        std::this_thread::sleep_for(250ms);
    }

    m_logger->Debug("BOT THREAD ENDED");
}

void Bot::on_connect() {
    m_logger->Info("Bot connected to the server.");
}

void Bot::on_receive(ENetPacket* pkt) {
    Packet rec_packet { pkt };

    m_logger->LogString(ILogger::LogType::Info, "Incoming " + rec_packet.ToString());

    enet_packet_destroy(pkt);


    switch (rec_packet.Type)
    {
    case ePacketType::NET_MESSAGE_GAME_PACKET: {
        if (rec_packet.TankPkt->Header.Type == eTankPacketType::NET_GAME_PACKET_CALL_FUNCTION) {
            VariantList varlist {};
            varlist.SerializeFromMem(rec_packet.TankPkt->Value.data(), static_cast<int>(rec_packet.TankPkt->Value.size()));
            
            on_incoming_varlist(varlist, *rec_packet.TankPkt);
            break;
        }
        on_incoming_tank_packet(*rec_packet.TankPkt);
        break;
    }
    default:
        on_incoming_text_packet(rec_packet.Type, *rec_packet.TextPkt);
        break;
    }
}

void Bot::on_disconnect() {
    m_logger->Info("Bot disconnected from the server.");

    if (m_is_redirected) {
        m_logger->Info("Reconnecting to sub-server");
        if (!Connect(m_server_ip, m_server_port, m_login_data.Meta, m_using_new_packet)) {
            m_logger->Error("Error occured while reconnecting to sub-server");
        }
        m_is_redirected = false;
        return;
    }

    if (m_reconnect || m_always_reconnect) {
        m_logger->Info("Auto reconnecting in 3 seconds");
        std::this_thread::sleep_for(3000ms);
        ConnectWithHttp();
        m_reconnect = false;
    }

    m_is_in_world = false;
    m_is_in_game = false;
}

void Bot::on_incoming_text_packet(ePacketType type, TextPacket pkt) {
    Utils::TextParse text_parse{ pkt, "\n" };

    switch (type)
    {
    case NET_MESSAGE_UNKNOWN:
        break;
    case NET_MESSAGE_SERVER_HELLO: {
        m_packet_handler_manager.HandleHelloPacket();
        break;
    }
    // it seems like the NET_MESSAGE_GENERIC_TEXT came from client and is not sent by the server.
    // kept for "just in case" situation
    case NET_MESSAGE_GENERIC_TEXT:
        m_packet_handler_manager.HandleGenericTextPacket(&pkt);
        break;
    case NET_MESSAGE_GAME_MESSAGE: {
        m_packet_handler_manager.HandleActionPacket(&pkt);
        break;
    }
    // seems unused
    case NET_MESSAGE_ERROR:
        m_packet_handler_manager.HandleErrorPacket(&pkt);
        break;
    case NET_MESSAGE_TRACK:
        m_packet_handler_manager.HandleTrackPacket(&pkt);
        break;
        // this seems unused too
    case NET_MESSAGE_CLIENT_LOG_REQUEST:
        m_packet_handler_manager.HandleLogRequestPacket(&pkt);
        break;
    default:
        break;
    }
}

void Bot::on_incoming_tank_packet(TankPacket pkt) {
    m_packet_handler_manager.HandleTankPacket(&pkt);
}

void Bot::on_incoming_varlist(VariantList varlist, TankPacket pkt) {
    m_packet_handler_manager.HandleVarlistPacket(&varlist, &pkt);
}
