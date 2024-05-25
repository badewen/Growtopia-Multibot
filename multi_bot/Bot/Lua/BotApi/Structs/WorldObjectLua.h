#pragma once

#include <lua/lua.h>

#include "../../../World/WorldItemObject.h"

class WorldObjectLua
{
public:
    static void PushAsTable(lua_State* L, WorldItemObject& obj);
};

