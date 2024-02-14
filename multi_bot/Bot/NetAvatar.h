#pragma once

#include <string>

#include "../Packet/PacketTypes.h"

struct NetAvatar
{
    std::string Name, CountryFlag;
    float PosX, PosY;
    float LastPosX, LastPosY;
    int32_t WidthSize, HeightSize;
    int32_t NetID;
    uint32_t UserID;
    bool IsInvis, FacingLeft;
    TankPacketFlag Flags;

    //local 
    std::string WorldName;
    int32_t Gems;
};