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
        OutputLog(
            type,
            fmt::format(format, arg...)
        );
    }

    void LogString(ILogger::LogType type, std::string msg) {
        OutputLog(
            type,
            msg
        );
    }

    template<typename ...Args>
    void Info(std::string format, Args&&... arg) { 
        Log(
            ILogger::LogType::Info,
            fmt::format(format, arg...)
        );
    }

    template<typename ...Args>
    void Debug(std::string format, Args&&... arg) {
        Log(
            ILogger::LogType::Debug,
            fmt::format(format, arg...)
        );
    }

    template<typename ...Args>
    void Error(std::string format, Args&&... arg) {
        Log(
            ILogger::LogType::Error,
            fmt::format(format, arg...)
        );
    }

protected:
    // handles the logging after getting formatted.
    // the type is just a hint to the log type. the formatted string is ready to be printer without any further modification
    virtual void OutputLog(ILogger::LogType type, std::string formatted) = 0;
};

