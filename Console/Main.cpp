#include <memory>
#include <chrono>

// BULDING FOR LINUX?
// https://github.com/casualsnek/linuxconio
#include <conio.h>

#include <spdlog/spdlog.h>

#include <Bot/Bot.h>
#include <Logger/ILogger.hpp>
#include <Network/Http/HttpClient.h>

#include <Utils/TextParse.h>

// this project is more of a place to test the bot.

using namespace std::chrono_literals;

bool g_output_log = true;
bool g_show_player_list = false;

class loger : public ILogger
{
protected:
    void ILogger::output_log(ILogger::LogType type, std::string log) {
        if (!g_output_log) { return; }
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


// https://stackoverflow.com/questions/11472043/non-blocking-getch
char non_block_getch() {
    if (_kbhit()) {
        return _getch();
    }
    return 0;
}

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

    bot.SetLoginGuest("02:34:12:43:ff:43", "394810af830294817ffaabb34857193");
    //bot.GenerateNewSpoof();
    //bot.SetLoginGrowID("GROWID", "PASS");
    bot.AlwaysReconnect(true);

    bot.ConnectWithHttp();

    bot.StartBotThread();

    while (!bot.IsInGame()) {
        std::this_thread::sleep_for(10ms);
    }

    std::this_thread::sleep_for(3000ms);

    while (true) {
        char pressed_key = non_block_getch();

        if (g_show_player_list) {
            // reduce screen flickering
            std::stringstream output_str = {};
            output_str << "\033[2J\033[;HPlayer name\tpos\n";
            output_str << bot.GetLocal()->Name << "\t\t" << bot.GetLocal()->PosX << ", " << bot.GetLocal()->PosY << "\n";
            for (auto player : bot.GetPlayerList()) {
                output_str << player.second.Name << "\t\t" << player.second.PosX << ", " << player.second.PosY << "\n";
            }

            std::cout << output_str.str();
        }

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
            // ubisoft nuked the tordawn312312 world :sad:
            bot.JoinWorld("tordawn3123122");
            break;
        }
        case 'L': {
            bot.JoinWorld("EXIT");
            break;
        }
        case '1': {
            g_output_log = true;
            g_show_player_list = false;
            printf("\033[2J");
            printf("\033[;H");
            break;
        }

        case '2': {
            g_output_log = false;
            g_show_player_list = true;
            printf("\033[2J");
            printf("\033[;H");
            break;
        }

        std::this_thread::sleep_for(100ms);
        }
    }

    return 0;
}