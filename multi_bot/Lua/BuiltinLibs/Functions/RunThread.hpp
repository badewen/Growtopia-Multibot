#pragma once

#include "../BuiltinLuaLib.h"

#include "../../../Bot/Bot.h"

int BuiltinLuaLib::l_run_thread(lua_State* L) {
    LUAHELPER_GET_LIB_DATA(LUABUILTINLIB_LUATHREADMGR_REG_UUID, LuaThreadManager*, lua_thread_mgr);
    
    luaL_checktype(L, 1, LUA_TFUNCTION);

    lua_State* thread_state = lua_newthread(L);
    int thread_refid = luaL_ref(L, LUA_REGISTRYINDEX);

    // copy the stack and lua_ref it to the registry index
    // this includes the function and the arguments.
    int args_count = lua_gettop(L);
    
    lua_newtable(L);

    for (int i = 0; i < args_count; i++) {
        lua_pushinteger(L, i + 1);
        lua_pushvalue(L, i + 1);
        lua_settable(L, -3);
    }

    int args_refid = luaL_ref(L, LUA_REGISTRYINDEX);

    lua_pop(L, args_count);

    std::thread dispatch_thread{
    [thread_state, thread_refid, args_refid, args_count, lua_thread_mgr]() {
        std::thread thread {
        [thread_state, thread_refid, args_refid, args_count, lua_thread_mgr]() {
            assert(lua_rawgeti(thread_state, LUA_REGISTRYINDEX, args_refid) == LUA_TTABLE);

            for (int i = 0; i < args_count; i++) {
                lua_pushinteger(thread_state, i + 1);
                lua_gettable(thread_state, 1);
            }

            if (lua_pcall(thread_state, args_count - 1, 0, 0) != LUA_OK) {
                lua_stopvm(thread_state, lua_tostring(thread_state, -1));
                lua_pop(thread_state, 1);
            }

            luaL_unref(thread_state, LUA_REGISTRYINDEX, args_refid);
            luaL_unref(thread_state, LUA_REGISTRYINDEX, thread_refid);
        } };

        lua_thread_mgr->LockThreadList();
        lua_thread_mgr->CleanupDeadThread();
        lua_thread_mgr->AddThread(thread_refid, std::move(thread));
        lua_thread_mgr->UnlockThreadList();
    } };

    dispatch_thread.detach();

    return 0;
}
