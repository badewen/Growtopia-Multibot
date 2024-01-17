#pragma once

#include <iostream>
#include <string>

#include <fmt/fmt.h>
#include <fmt/chrono.h>

class ILogger
{
public:
    enum class LogType {
        Info,
        Error,
        Debug
    };
public:
    ILogger() = default;

    template<typename ...Args>
    void Log(ILogger::LogType type, std::string format, Args&&... arg) {
        LogFormatted(
            type,
            fmt::format("[{:%Y-%m-%d %H:%M:%S.%e}] ", fmt::localtime(std::time(nullptr)) +
            fmt::format(format, ...arg)
        );
    }

    template<typename ...Args>
    void Info(std::string format, Args&&... arg) { 
        Log(
            ILogger::LogType::Info,
            "[INFO] " +
            fmt::format(format, ...arg)
        );
    }

protected:
    // handles the logging after getting formatted.
    // the type is just a hint to the log type. the formatted string is ready to be printer without any further modification
    virtual void LogFormatted(ILogger::LogType type, std::string formatted) = 0;
};

