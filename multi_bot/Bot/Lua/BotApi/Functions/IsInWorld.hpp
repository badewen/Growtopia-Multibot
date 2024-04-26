#pragma once

#include "../LuaBotApiLib.h"

#include "../../../Bot.h"

int LuaBotApiLib::l_is_in_world(lua_State* L) {
    LUAHELPER_GET_LIB_DATA(LUABOTAPILIB_BOT_REG_UUID, Bot*, bot);

    lua_pushboolean(L, bot->IsInGame());

    return 1;
}
