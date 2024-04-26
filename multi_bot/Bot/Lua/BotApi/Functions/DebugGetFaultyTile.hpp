#pragma once

#include "../LuaBotApiLib.h"

#include "../../../Bot.h"

#include "../Structs/NetAvatarLua.h"

int LuaBotApiLib::l_debug_get_faulty_tile(lua_State* L) {
    LUAHELPER_GET_LIB_DATA(LUABOTAPILIB_BOT_REG_UUID, Bot*, bot);

    lua_newtable(L);

    if (bot->GetCurrentWorldRef().GetFaultyTile().Fg != 0) {
        lua_pushstring(L, "id");
        lua_pushinteger(L, bot->GetCurrentWorldRef().GetFaultyTile().Fg);
        lua_settable(L, -3);

        lua_pushstring(L, "extra_tile_type");
        lua_pushinteger(L, bot->GetCurrentWorldRef().GetFaultyTile().ExtraTileDataType);
        lua_settable(L, -3);
    }

    return 1;
}
