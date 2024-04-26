#include "NetAvatarLua.h"

#include "../../../../Lua/LuaHelper.h"

void NetAvatarLua::PushAsTable(lua_State* L, const NetAvatar* net_avatar, bool is_local) {
    lua_newtable(L);

    LUAHELPER_SET_TABLE(string, "name", net_avatar->Name.c_str());
    LUAHELPER_SET_TABLE(string, "country", net_avatar->CountryFlag.c_str());
    LUAHELPER_SET_TABLE(number, "pos_x", net_avatar->PosX);
    LUAHELPER_SET_TABLE(number, "pos_y", net_avatar->PosY);
    LUAHELPER_SET_TABLE(integer, "width", net_avatar->WidthSize);
    LUAHELPER_SET_TABLE(integer, "height", net_avatar->HeightSize)
    LUAHELPER_SET_TABLE(integer, "net_id", net_avatar->NetID)
    LUAHELPER_SET_TABLE(integer, "user_id", net_avatar->UserID)
    LUAHELPER_SET_TABLE(boolean, "is_invis", net_avatar->IsInvis);
    LUAHELPER_SET_TABLE(boolean, "is_facing_left", net_avatar->FacingLeft);
    LUAHELPER_SET_TABLE(boolean, "is_local", is_local);
    LUAHELPER_SET_TABLE(integer, "flags", *(uint32_t*)&net_avatar->Flags);
}
