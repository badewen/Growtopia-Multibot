#pragma once

#define LUAHELPER_ADD_GLOBAL_FUNC(L, func, name) lua_pushcfunction(L, func); lua_setglobal(L, name);
#define LUAHELPER_RMOV_GLOBAL_FUNC(L, name) lua_pushnil(L); lua_setglobal(L, name);

#define LUAHELPER_GET_LIB_DATA(uuid, type, var)\
    lua_pushstring(L, uuid);\
    lua_gettable(L, LUA_REGISTRYINDEX);\
    type var = (type)lua_touserdata(L, -1);\
    lua_pop(L, 1);

#define LUAHELPER_SET_TABLE(type, field, data)\
    lua_pushstring(L, field);\
    lua_push##type(L, data);\
    lua_settable(L, -3);\

#define LUAHELPER_GET_TABLE(luatype, field, var)\
    lua_pushstring(L, field);\
    lua_gettable(L, -2);\
    var = static_cast<decltype(var)>(lua_to##luatype(L, -1));\
    lua_pop(L, 1);

