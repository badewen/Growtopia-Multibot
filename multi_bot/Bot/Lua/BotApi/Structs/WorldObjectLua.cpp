#include "WorldObjectLua.h"

#include "../../../../Lua/LuaHelper.h"

void WorldObjectLua::PushAsTable(lua_State* L, WorldItemObject& obj) {
    lua_newtable(L);

    LUAHELPER_SET_TABLE(integer, "oid", obj.ObjectID);
    LUAHELPER_SET_TABLE(integer, "id", obj.ItemID);
    LUAHELPER_SET_TABLE(integer, "count", obj.ItemAmount);
    LUAHELPER_SET_TABLE(number, "pos_x", obj.PosX);
    LUAHELPER_SET_TABLE(number, "pos_y", obj.PosY);
    LUAHELPER_SET_TABLE(integer, "flags", *(uint8_t*)&obj.Flags);
}
