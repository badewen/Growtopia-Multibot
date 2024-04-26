#pragma once

#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>

#include "LuaHelper.h"

class ILuaLibrary
{
public:
    // idk do any library specific / state loading here
    virtual void OnLoadLibrary(lua_State* L) = 0;
    // properly unload a library
    virtual void OnUnloadLibrary(lua_State* L) {}
};
