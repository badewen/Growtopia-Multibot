#pragma once

#include <lua/lua.h>

#include "../../../NetAvatar.h"

class NetAvatarLua
{
public:
    static void PushAsTable(lua_State* L, const NetAvatar* local, bool is_local);
};

