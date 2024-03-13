#pragma once

#include <stdint.h>

using WorldItemObjectID = uint32_t;

struct WorldItemObjectFlag {
    uint8_t ShrinkOnTaken : 1;
    uint8_t Unk1 : 2;
    uint8_t RadioactiveItem : 1;
};

struct WorldItemObject
{
    WorldItemObjectID ObjectID;
    uint32_t ItemID;
    uint32_t ItemAmount;
    float PosX, PosY;
    WorldItemObjectFlag Flags;
};