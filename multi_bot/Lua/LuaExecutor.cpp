#include "LuaExecutor.h"

#include <functional>

void LuaExecutor::AddLibrary(std::shared_ptr<ILuaLibrary> lib) {
    m_enabled_library.push_back(lib);
}

LuaExecutorError LuaExecutor::ExecuteString(std::string script) {
    LuaExecutorError ret{};

    if (m_current_lua_state) {
        ret.ErrorCode = eLuaExecutorError::AScriptIsRunning;
        ret.ErrorMessage = "A script is already running! Terminate the previous script first!!";
        return ret;
    }

    m_current_lua_state = luaL_newstate();

    ret.ErrorCode = (eLuaExecutorError)luaL_loadstring(m_current_lua_state, script.c_str());

    luaL_openlibs(m_current_lua_state);

    for (auto& lib : m_enabled_library) {
        lib->OnLoadLibrary(m_current_lua_state);
    }
    
    if (ret.ErrorCode != eLuaExecutorError::NoError) {
        ret.ErrorMessage = lua_tostring(m_current_lua_state, -1);
        lua_pop(m_current_lua_state, 1);
        return ret;
    }

    ret.ErrorCode = (eLuaExecutorError)lua_pcall(m_current_lua_state, 0, LUA_MULTRET, 0);

    for (auto& lib : m_enabled_library) {
        lib->OnUnloadLibrary(m_current_lua_state);
    }

    lua_stopvm(m_current_lua_state, "Script finished");

    if (ret.ErrorCode != eLuaExecutorError::NoError) {
        ret.ErrorMessage = lua_tostring(m_current_lua_state, -1);
        lua_pop(m_current_lua_state, 1);
        lua_close(m_current_lua_state);
        m_current_lua_state = nullptr;
        return ret;
    }

    lua_close(m_current_lua_state);
    m_current_lua_state = nullptr;
    return ret;
}

void LuaExecutor::StopScript(std::string reason) {
    if (m_current_lua_state) {
        lua_stopvm(m_current_lua_state, reason);
        m_current_lua_state = nullptr;
    }
}
