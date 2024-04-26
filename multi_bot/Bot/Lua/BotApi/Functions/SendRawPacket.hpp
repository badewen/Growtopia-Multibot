#pragma once

#include "../LuaBotApiLib.h"

#include "../../../Bot.h"

#include "../Structs/TankPacketLua.h"

int LuaBotApiLib::l_send_raw_packet(lua_State* L) {
    LUAHELPER_GET_LIB_DATA(LUABOTAPILIB_BOT_REG_UUID, Bot*, bot);

    TankPacket pkt{};

    luaL_checktype(L, 1, LUA_TTABLE);

    pkt = TankPacketLua::GetFromTable(L);

    bot->SendPacket({ pkt });
    
    return 0;
}
