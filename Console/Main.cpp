#include <memory>
#include <chrono>
#include <conio.h>

#include <spdlog/spdlog.h>

#include <Bot/Bot.h>
#include <Logger/ILogger.h>
#include <Network/Http/HttpClient.h>

#include <Utils/TextParse.h>

// this project is more of a place to test the bot.

using namespace std::chrono_literals;

class loger : public ILogger
{
protected:
    void ILogger::OutputLog(ILogger::LogType type, std::string log) {
        switch (type)
        {
        case ILogger::LogType::Info: {
            spdlog::info(log);
            break;
        }
        case ILogger::LogType::Error: {
            spdlog::error(log);
            break;
        }
        case ILogger::LogType::Debug: {
            spdlog::debug(log);
            break;
        }
        default:
            break;
        }
    }
};

int main() {
    spdlog::set_level(spdlog::level::debug);

    if (enet_initialize() != 0)
    {
        spdlog::critical("An error occurred while initializing ENet.\n");
        return EXIT_FAILURE;
    }
    Bot bot {std::move(std::make_unique<loger>())};

    if (!bot.CreateHost()) {
        spdlog::info("Failed to create host");
    }

    bot.SetLoginGuest("MAC", "RID");
    //bot.SetLoginGrowID("GROWID", "PASS");
    bot.AlwaysReconnect(true);

    bot.ConnectWithHttp();

    bot.StartBotThread();

    while (!bot.IsInGame()) {
        std::this_thread::sleep_for(10ms);
    }

    std::this_thread::sleep_for(3000ms);

    while (true) {
        
        char pressed_key = _getch();

        switch (pressed_key)
        {
        case 'W': {
            bot.Move(0, -1);
            break;
        }
        case 'A': {
            bot.Move(-1, 0);
            break;
        }
        case 'S': {
            bot.Move(0, 1);
            break;
        }
        case 'D': {
            bot.Move(1, 0);
            break;
        }
        case 'J': {
            bot.JoinWorld("tordawn312312");
            break;
        }
        case 'L': {
            bot.JoinWorld("EXIT");
            break;
        }
        default:
            break;
        }

        std::this_thread::sleep_for(100ms);
    }

    return 0;
}