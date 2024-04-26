#pragma once

#include "../BuiltinLuaLib.h"
#include "../../../Network/Http/HttpClient.h"

int BuiltinLuaLib::l_send_webhook(lua_State* L) {
    luaL_checktype(L, 1, LUA_TSTRING);
    luaL_checktype(L, 2, LUA_TSTRING);

    LUAHELPER_GET_LIB_DATA(LUABUILTINLIB_HTTPCLIENT_REG_UUID, HttpClient*, http_cl);

    http_cl->Post(lua_tostring(L, 1), lua_tostring(L, 2), {}, "application/json");

    lua_pop(L, 2);

    return 0;
}
