#pragma once

#include "../LuaBotApiLib.h"

#include "../../../Bot.h"

int LuaBotApiLib::l_add_hook(lua_State* L) {
    luaL_checktype(L, 1, LUA_TSTRING);
    luaL_checktype(L, 2, LUA_TSTRING);
    luaL_checktype(L, 3, LUA_TFUNCTION);
   
    if (strcmp(lua_tostring(L, 1), "OnTextPacket") &&
        strcmp(lua_tostring(L, 1), "OnTankPacket") &&
        strcmp(lua_tostring(L, 1), "OnVarlistPacket")
    ) {
        lua_pop(L, 3);
        return 0;
    }

    if (!strcmp(lua_tostring(L, 1), "OnTextPacket")) {
        lua_pushstring(L, LUABOTAPILIB_TEXTPKT_CB_REG_UUID);
    }
    else if (!strcmp(lua_tostring(L, 1), "OnTankPacket")) {
        lua_pushstring(L, LUABOTAPILIB_TANKPKT_CB_REG_UUID);
    }
    else if (!strcmp(lua_tostring(L, 1), "OnVarlistPacket")) {
        lua_pushstring(L, LUABOTAPILIB_VARLISTPKT_CB_REG_UUID);
    }
    else {
        throw("wtf report to dev idk how it happen here. " __FILE__);
    }

    lua_gettable(L, LUA_REGISTRYINDEX); // get the textpkt callbacks table

    lua_pushvalue(L, 2); // the hook id
    lua_pushvalue(L, 3); // the hook function

    lua_settable(L, -3); // set the hook id and the table into textpkt callbacks table

    lua_pop(L, 4);

    return 0;
}
