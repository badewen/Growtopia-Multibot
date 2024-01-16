#pragma once

#include <iostream>
#include <string>

#include <fmt/fmt.h>
#include <fmt/chrono.h>

class ILogger
{
public:
    ILogger() = default;

    template<typename ...Args>
    void Log(std::string format, Args&&... arg) {
        LogFormattedRaw(
            fmt::format("[{:%Y-%m-%d %H:%M:%S.%e}] ", fmt::localtime(std::time(nullptr)) +
            fmt::format(format, ...arg)
        );
    }

    template<typename ...Args>
    void Info(std::string format, Args&&... arg) { 
        Log(
            "[INFO] " +
            fmt::format(format, ...arg)
        );
    }

protected:
    // handles the logging after getting formatted.
    virtual void LogFormattedRaw(std::string formatted) = 0;
};

