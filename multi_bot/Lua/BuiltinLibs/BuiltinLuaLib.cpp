#include "BuiltinLuaLib.h"

#include "Functions/Sleep.hpp"
#include "Functions/RunThread.hpp"
#include "Functions/SendWebhook.hpp"

void BuiltinLuaLib::OnLoadLibrary(lua_State* L) {
    lua_pushstring(L, LUABUILTINLIB_HTTPCLIENT_REG_UUID);
    lua_pushlightuserdata(L, new HttpClient{""});
    lua_settable(L, LUA_REGISTRYINDEX);

    LUAHELPER_ADD_GLOBAL_FUNC(L, l_sleep, "Sleep");
    LUAHELPER_ADD_GLOBAL_FUNC(L, l_run_thread, "RunThread");
    LUAHELPER_ADD_GLOBAL_FUNC(L, l_send_webhook, "SendWebhook");
}

void BuiltinLuaLib::OnUnloadLibrary(lua_State* L) {
    LUAHELPER_RMOV_GLOBAL_FUNC(L, "Sleep");
    LUAHELPER_RMOV_GLOBAL_FUNC(L, "RunThread");
    LUAHELPER_RMOV_GLOBAL_FUNC(L, "SendWebhook");

    LUAHELPER_GET_LIB_DATA(LUABUILTINLIB_HTTPCLIENT_REG_UUID, HttpClient*, http_cl);
    delete http_cl;

    lua_pushstring(L, LUABUILTINLIB_HTTPCLIENT_REG_UUID);
    lua_pushnil(L);
    lua_settable(L, LUA_REGISTRYINDEX);
}
