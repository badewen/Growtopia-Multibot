#pragma once

#include "../BuiltinLuaLib.h"

#include <chrono>
#include <thread>

int BuiltinLuaLib::l_sleep(lua_State* L) {

    luaL_checktype(L, 1, LUA_TNUMBER);

    lua_Number sleep_ms = lua_tonumber(L, 1);
    lua_pop(L, 1);

    // check every 100 ms
    for (size_t n = 0; n < floor(sleep_ms / 100); n++) {
        if (lua_isvmstopped(L)) {
            return 0;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds{100});
    }
    
    std::this_thread::sleep_for(std::chrono::milliseconds{ static_cast<uint64_t>(floorf(fmod(sleep_ms, 100.0))) });

    return 0;
}
