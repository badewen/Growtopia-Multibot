#pragma once

#include "../LuaBotApiLib.h"

#include "../../../Bot.h"
#include "../Structs/WorldObjectLua.h"

int LuaBotApiLib::l_get_objects(lua_State* L) {
    LUAHELPER_GET_LIB_DATA(LUABOTAPILIB_BOT_REG_UUID, Bot*, bot);

    std::vector<WorldItemObject> objects = bot->GetCurrentWorldRef().GetItemObjects();

    lua_newtable(L);

    for (size_t i = 0; i < objects.size(); i++) {
        lua_pushnumber(L, i);
        WorldObjectLua::PushAsTable(L, objects[i]);
        lua_settable(L, -3);
    }

    return 1;
}
