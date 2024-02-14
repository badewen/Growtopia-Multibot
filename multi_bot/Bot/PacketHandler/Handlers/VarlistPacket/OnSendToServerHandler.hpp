#pragma once

#include <proton/Variant.h>

#include "../../../Bot.h"
#include "../../IPacketHandler.hpp"
#include "../../../../Utils/TextParse.h"
#include "../../../RedirectServerData.h"

// it is getting too long to handle the -1s in the ONSendServer
#define DONT_CHANGE_VAR_IF(var, text, cond) var = text cond ? var : text

class OnSendToServerHandler : public IPacketHandler
{
public:
    OnSendToServerHandler(Bot* bot_obj) : IPacketHandler::IPacketHandler(bot_obj) {}

    void Handle(const VariantList* varlist, const TankPacket* tank_pkt) override {
        m_bot->SetIsRedirected(true);

        m_bot->GetLoggerPtr()->Debug("Got server redirect");

        m_bot->Disconnect();

        std::vector<std::string> server_data{
            Utils::TextParse::StringTokenize(varlist->m_variant[4].GetString(), "|")
        };

        RedirectServerData redirect_server_data{};

        redirect_server_data.UUIDToken = server_data.at(2) == "-1" ? redirect_server_data.UUIDToken : server_data.at(2);
        redirect_server_data.Token = std::to_string(varlist->m_variant[2].GetINT32()) == "-1" ? redirect_server_data.Token : std::to_string(varlist->m_variant[2].GetINT32());

        redirect_server_data.DoorID = server_data.at(1).empty() ? "0" : server_data.at(1);
        redirect_server_data.UserID = std::to_string(varlist->m_variant[3].GetINT32());

        m_bot->SetRedirectData(&redirect_server_data);
        m_bot->SetServerIp(server_data.at(0));
        m_bot->SetServerPort(std::to_string(varlist->m_variant[1].GetINT32()));
    }
};