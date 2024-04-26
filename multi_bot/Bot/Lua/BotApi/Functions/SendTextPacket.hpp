#pragma once

#include "../LuaBotApiLib.h"

#include "../../../Bot.h"

int LuaBotApiLib::l_send_text_packet(lua_State* L) {
    LUAHELPER_GET_LIB_DATA(LUABOTAPILIB_BOT_REG_UUID, Bot*, bot);

    luaL_checktype(L, 1, LUA_TNUMBER);
    luaL_checktype(L, 2, LUA_TSTRING);

    Packet pkt{};
    
    pkt.Type = (ePacketType)lua_tointeger(L, 1);
    pkt.TextPkt = std::make_unique<TextPacket>(lua_tostring(L, 2));
    
    lua_pop(L, 2);

    bot->SendPacket(std::move(pkt));

    return 0;
}
