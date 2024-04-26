#pragma once

#include <lua/lua.h>

#include "../../../../Packet/Packet.h"

class TankPacketLua
{
public:
    static void PushAsTable(lua_State* L, TankPacket& pkt);
    static TankPacket GetFromTable(lua_State* L);
};

