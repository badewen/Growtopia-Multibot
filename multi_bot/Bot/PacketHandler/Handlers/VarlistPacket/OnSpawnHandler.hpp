#pragma once

#include <proton/Variant.h>

#include "../../../Bot.h"
#include "../../../NetAvatar.h"
#include "../../IPacketHandler.hpp"
#include "../../../../Utils/TextParse.h"

class OnSpawnHandler : public IPacketHandler
{
public:
    OnSpawnHandler(Bot* bot_obj) : IPacketHandler::IPacketHandler(bot_obj) {}

    void Handle(const VariantList* varlist, const TankPacket* tank_pkt) override {
        Utils::TextParse text_parse{ varlist->m_variant[1].GetString(), "\n"};
        int32_t netid = std::stoi(text_parse.Get("netID"));
        int32_t userid = std::stoi(text_parse.Get("userID"));

        int32_t width = std::stoi(text_parse.Get("colrect", 3));
        int32_t height = std::stoi(text_parse.Get("colrect", 4));
        float x = std::stof(text_parse.Get("posXY", 1));
        float y = std::stof(text_parse.Get("posXY", 2));

        std::string name = text_parse.Get("name");
        std::string country = text_parse.Get("country");

        NetAvatar* bot_local_ptr = m_bot->GetLocalPtr();

        if (text_parse.Get("type") == "local") {
            bot_local_ptr->NetID = netid;
            bot_local_ptr->UserID = userid;
            bot_local_ptr->WidthSize = width;
            bot_local_ptr->HeightSize = height;
            bot_local_ptr->Name = name;
            bot_local_ptr->CountryFlag = country;
            bot_local_ptr->PosX = x;
            bot_local_ptr->PosY = y;
            bot_local_ptr->LastPosX = x;
            bot_local_ptr->LastPosY = y;
        }
        else {
            NetAvatar temp;
            temp.NetID = netid;
            temp.UserID = userid;
            temp.WidthSize = width;
            temp.HeightSize = height;
            temp.Name = name;
            temp.CountryFlag = country;
            temp.PosX = x;
            temp.PosY = y;
            temp.IsInvis = std::stoi(text_parse.Get("invis"));
            temp.LastPosX = x;
            temp.LastPosY = y;

            m_bot->GetPlayerListPtr()->insert_or_assign(netid, temp);
        }
    }
};