#pragma once

#include <lua/lua.h>
#include <proton/Variant.h>

class VarlistPacketLua
{
public:
    static void PushAsTable(lua_State* L, VariantList& varlist);
};

