#pragma once

#include "../ILuaLibrary.h"

#include "../../Network/Http/HttpClient.h"

#define LUABUILTINLIB_HTTPCLIENT_REG_UUID "24ff5a7d-4f0a"
#define LUABUILTINLIB_LUATHREADMGR_REG_UUID "d4356871-ff78"

class BuiltinLuaLib : public ILuaLibrary
{
public:
    void OnLoadLibrary(lua_State* L) override;
    void OnUnloadLibrary(lua_State* L) override;
private:

    // Sleep(int ms)
    static int l_sleep(lua_State* L);

    // RunThread(function, args ...) 
    static int l_run_thread(lua_State* L);

    // SendWebhook(string url, string json);
    static int l_send_webhook(lua_State* L);
};

