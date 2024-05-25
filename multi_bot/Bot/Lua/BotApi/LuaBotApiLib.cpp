#include "LuaBotApiLib.h"

#include "Functions/GetLocal.hpp"
#include "Functions/SendRawPacket.hpp"
#include "Functions/SendTextPacket.hpp"
#include "Functions/IsInGame.hpp"
#include "Functions/IsInWorld.hpp"
#include "Functions/AddHook.hpp"
#include "Functions/GetObjects.hpp"
#include "Functions/DebugGetFaultyTile.hpp"

#include "../../../Packet/Packet.h"

#include "Structs/TankPacketLua.h"
#include "Structs/VarlistPacketLua.h"

void LuaBotApiLib::OnLoadLibrary(lua_State* L) {
    lua_pushstring(L, LUABOTAPILIB_BOT_REG_UUID);
    lua_pushlightuserdata(L, m_bot);
    lua_settable(L, LUA_REGISTRYINDEX);      

    lua_pushstring(L, LUABOTAPILIB_TEXTPKT_CB_REG_UUID);
    lua_newtable(L);
    lua_settable(L, LUA_REGISTRYINDEX);
    
    lua_pushstring(L, LUABOTAPILIB_TANKPKT_CB_REG_UUID);
    lua_newtable(L);
    lua_settable(L, LUA_REGISTRYINDEX);

    lua_pushstring(L, LUABOTAPILIB_VARLISTPKT_CB_REG_UUID);
    lua_newtable(L);
    lua_settable(L, LUA_REGISTRYINDEX);

    LUAHELPER_ADD_GLOBAL_FUNC(L, l_send_raw_packet, "SendRawPacket");
    LUAHELPER_ADD_GLOBAL_FUNC(L, l_send_text_packet, "SendTextPacket");
    LUAHELPER_ADD_GLOBAL_FUNC(L, l_get_local, "GetLocal");
    LUAHELPER_ADD_GLOBAL_FUNC(L, l_is_in_game, "IsInGame");
    LUAHELPER_ADD_GLOBAL_FUNC(L, l_is_in_world, "IsInWorld");
    LUAHELPER_ADD_GLOBAL_FUNC(L, l_add_hook, "AddHook");
    LUAHELPER_ADD_GLOBAL_FUNC(L, l_get_objects, "GetObjects");

#ifdef _DEBUG
    LUAHELPER_ADD_GLOBAL_FUNC(L, l_debug_get_faulty_tile, "DebugGetFaultyTile");
#endif

    m_text_pkt_cb_thread_state = lua_newthread(L);
    m_text_pkt_cb_thread_refid = luaL_ref(L, LUA_REGISTRYINDEX);

    m_tank_pkt_cb_thread_state = lua_newthread(L);
    m_tank_pkt_cb_thread_refid = luaL_ref(L, LUA_REGISTRYINDEX);

    m_var_pkt_cb_thread_state = lua_newthread(L);
    m_var_pkt_cb_thread_refid = luaL_ref(L, LUA_REGISTRYINDEX);

    lua_pushstring(L, LUABOTAPILIB_ONPKT_INVOKER_REG_UUID);

    luaL_dostring(L, R"(
return function(cb_table, ...)
    for _, func in pairs(cb_table) do
        RunThread(func, table.unpack({...}))
    end
end)");

    lua_settable(L, LUA_REGISTRYINDEX);
}

void LuaBotApiLib::OnUnloadLibrary(lua_State* L) {
    lua_pushstring(L, LUABOTAPILIB_TEXTPKT_CB_REG_UUID);
    lua_pushnil(L);
    lua_settable(L, LUA_REGISTRYINDEX);

    lua_pushstring(L, LUABOTAPILIB_TANKPKT_CB_REG_UUID);
    lua_pushnil(L);
    lua_settable(L, LUA_REGISTRYINDEX);

    lua_pushstring(L, LUABOTAPILIB_VARLISTPKT_CB_REG_UUID);
    lua_pushnil(L);
    lua_settable(L, LUA_REGISTRYINDEX);

    LUAHELPER_RMOV_GLOBAL_FUNC(L, "SendRawPacket");
    LUAHELPER_RMOV_GLOBAL_FUNC(L, "SendTextPacket");
    LUAHELPER_RMOV_GLOBAL_FUNC(L, "GetLocal");
    LUAHELPER_RMOV_GLOBAL_FUNC(L, "IsInGame");
    LUAHELPER_RMOV_GLOBAL_FUNC(L, "IsInWorld");
    LUAHELPER_RMOV_GLOBAL_FUNC(L, "AddHook");
    LUAHELPER_RMOV_GLOBAL_FUNC(L, "GetObjects");

#ifdef _DEBUG
    LUAHELPER_RMOV_GLOBAL_FUNC(L, "DebugGetFaultyTile");
#endif

    lua_pushstring(L, LUABOTAPILIB_BOT_REG_UUID);
    lua_pushnil(L);
    lua_settable(L, LUA_REGISTRYINDEX);

    lua_pushstring(L, LUABOTAPILIB_ONPKT_INVOKER_REG_UUID);
    lua_pushnil(L);
    lua_settable(L, LUA_REGISTRYINDEX);

    m_text_pkt_cb_thread_state = nullptr;
    luaL_unref(L, LUA_REGISTRYINDEX, m_text_pkt_cb_thread_refid);

    m_tank_pkt_cb_thread_state = nullptr;
    luaL_unref(L, LUA_REGISTRYINDEX, m_tank_pkt_cb_thread_refid);

    m_var_pkt_cb_thread_state = nullptr;
    luaL_unref(L, LUA_REGISTRYINDEX, m_var_pkt_cb_thread_refid);
}

void LuaBotApiLib::TryInvokeOnTextPktCallback(lua_State* L, ePacketType pkt_type, TextPacket pkt) {
    lua_pushstring(m_text_pkt_cb_thread_state, LUABOTAPILIB_ONPKT_INVOKER_REG_UUID);
    lua_gettable(m_text_pkt_cb_thread_state, LUA_REGISTRYINDEX);

    lua_pushstring(m_text_pkt_cb_thread_state, LUABOTAPILIB_TEXTPKT_CB_REG_UUID);
    lua_gettable(m_text_pkt_cb_thread_state, LUA_REGISTRYINDEX);

    lua_pushinteger(m_text_pkt_cb_thread_state, pkt_type);
    lua_pushstring(m_text_pkt_cb_thread_state, pkt.c_str());

    if (lua_pcall(m_text_pkt_cb_thread_state, 3, 0, 0) != LUA_OK) {
        lua_stopvm(m_text_pkt_cb_thread_state, lua_tostring(m_text_pkt_cb_thread_state, -1));
        lua_pop(m_text_pkt_cb_thread_state, 1);
    }
}


void LuaBotApiLib::TryInvokeOnTankPktCallback(lua_State* L, TankPacket pkt) {
    lua_pushstring(m_tank_pkt_cb_thread_state, LUABOTAPILIB_ONPKT_INVOKER_REG_UUID);
    lua_gettable(m_tank_pkt_cb_thread_state, LUA_REGISTRYINDEX);

    lua_pushstring(m_tank_pkt_cb_thread_state, LUABOTAPILIB_TANKPKT_CB_REG_UUID);
    lua_gettable(m_tank_pkt_cb_thread_state, LUA_REGISTRYINDEX);

    TankPacketLua::PushAsTable(m_tank_pkt_cb_thread_state, pkt);

    if (lua_pcall(m_tank_pkt_cb_thread_state, 2, 0, 0) != LUA_OK) {
        lua_stopvm(m_tank_pkt_cb_thread_state, lua_tostring(m_tank_pkt_cb_thread_state, -1));
        lua_pop(m_tank_pkt_cb_thread_state, 1);
    }
}

void LuaBotApiLib::TryInvokeOnVarlistPktCallback(lua_State* L, TankPacket pkt, VariantList varlist) {
    lua_pushstring(m_var_pkt_cb_thread_state, LUABOTAPILIB_ONPKT_INVOKER_REG_UUID);
    lua_gettable(m_var_pkt_cb_thread_state, LUA_REGISTRYINDEX);

    lua_pushstring(m_var_pkt_cb_thread_state, LUABOTAPILIB_VARLISTPKT_CB_REG_UUID);
    lua_gettable(m_var_pkt_cb_thread_state, LUA_REGISTRYINDEX);

    TankPacketLua::PushAsTable(m_var_pkt_cb_thread_state, pkt);
    VarlistPacketLua::PushAsTable(m_var_pkt_cb_thread_state, varlist);

    if (lua_pcall(m_var_pkt_cb_thread_state, 3, 0, 0) != LUA_OK) {
        lua_stopvm(m_var_pkt_cb_thread_state, lua_tostring(m_var_pkt_cb_thread_state, -1));
        lua_pop(m_var_pkt_cb_thread_state, 1);
    }
}
