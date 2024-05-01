#include "VarlistPacketLua.h"

#include "../../../../Lua/LuaHelper.h"

void VarlistPacketLua::PushAsTable(lua_State* L, VariantList& varlist) {
    lua_newtable(L);
    for (uint32_t i = 0; i < C_MAX_VARIANT_LIST_PARMS; i++) {
        lua_pushinteger(L, i);

        const Variant& curr_variant = varlist.m_variant[i];

        switch (curr_variant.GetType())
        {
        case eVariantType::TYPE_FLOAT: {
            lua_pushnumber(L, curr_variant.GetFloat());
            break;
        }

        case eVariantType::TYPE_UINT32: {
            lua_pushinteger(L, curr_variant.GetUINT32());
            break;
        }

        case eVariantType::TYPE_INT32: {
            lua_pushinteger(L, curr_variant.GetINT32());
            break;
        }

        case eVariantType::TYPE_STRING: {
            lua_pushstring(L, curr_variant.GetString().c_str());
            break;
        }

        case eVariantType::TYPE_RECT: {
            lua_newtable(L);
            LUAHELPER_SET_TABLE(number, "x", curr_variant.GetRect().x);
            LUAHELPER_SET_TABLE(number, "y", curr_variant.GetRect().y);
            LUAHELPER_SET_TABLE(number, "width", curr_variant.GetRect().width);
            LUAHELPER_SET_TABLE(number, "height", curr_variant.GetRect().height);
            break;
        }

        case eVariantType::TYPE_VECTOR2: {
            lua_newtable(L);
            LUAHELPER_SET_TABLE(number, "x", curr_variant.GetVector2().x);
            LUAHELPER_SET_TABLE(number, "y", curr_variant.GetVector2().y);
            break;
        }

        case eVariantType::TYPE_VECTOR3: {
            lua_newtable(L);
            LUAHELPER_SET_TABLE(number, "x", curr_variant.GetVector3().x);
            LUAHELPER_SET_TABLE(number, "y", curr_variant.GetVector3().y);
            LUAHELPER_SET_TABLE(number, "z", curr_variant.GetVector3().z);
            break;
        }

        default: {
            lua_pushnil(L);
            break;
        }

        }

        lua_settable(L, -3);
    }
}
