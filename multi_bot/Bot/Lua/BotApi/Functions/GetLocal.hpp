#pragma once

#include "../LuaBotApiLib.h"

#include "../../../Bot.h"

#include "../Structs/NetAvatarLua.h"

int LuaBotApiLib::l_get_local(lua_State* L) {
    LUAHELPER_GET_LIB_DATA(LUABOTAPILIB_BOT_REG_UUID, Bot*, bot);

    NetAvatar* local = bot->GetLocalPtr();

    NetAvatarLua::PushAsTable(L, local, true);

    return 1;
}
