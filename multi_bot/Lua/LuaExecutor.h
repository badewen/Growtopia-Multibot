#pragma once

#include <string>
#include <vector>
#include <optional>
#include <memory>

#include "ILuaLibrary.h"

enum eLuaExecutorError
{
    // compatible with lua status code.
    NoError = 0,
    ScriptRuntimeError = 2,
    SyntaxError = 3,
    MemoryError = 4,
    ScriptFileError = 6,

    AScriptIsRunning,
};

struct LuaExecutorError
{
    eLuaExecutorError ErrorCode;
    // is set if ErrorCode is not NoError
    std::string ErrorMessage;
};

class LuaExecutor
{
public:
    // the library WILL only load when a script is executed.
    void AddLibrary(std::shared_ptr<ILuaLibrary> lib);
    
    // wont return until either script finished or it is stopped by StopScript.
    LuaExecutorError ExecuteString(std::string script);
    void StopScript(std::string reason);

    std::optional<lua_State*> GetCurrentLuaState() { return m_current_lua_state ? m_current_lua_state : std::optional<lua_State*>{}; }

private:
    std::vector<std::shared_ptr<ILuaLibrary>> m_enabled_library{};
    lua_State* m_current_lua_state{};
};

