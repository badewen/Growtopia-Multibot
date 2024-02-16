#include <memory>
#include <chrono>

// BULDING FOR LINUX?
// https://github.com/casualsnek/linuxconio
#include <conio.h>

#include <spdlog/spdlog.h>

#include <Bot/BotManager.h>
#include <Logger/ILogger.hpp>
#include <Network/Http/HttpClient.h>

#include <Utils/TextParse.h>

// this project is more of a place to test the curr_bot->

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

    BotManager bot_mgr {};
    std::shared_ptr<Bot> curr_bot = bot_mgr.AddBot("test_bot", std::make_shared<loger>());

    if (!curr_bot->CreateHost()) {
        spdlog::info("Failed to create host");
    }

    curr_bot->SetLoginGuest("MAC", "RID");
    //curr_bot->GenerateNewSpoof();
    //curr_bot->SetLoginGrowID("GROWID", "PASS");
    curr_bot->AlwaysReconnect(true);

    curr_bot->ConnectWithHttp();

    curr_bot->StartBotThread();

    while (!curr_bot->IsInGame()) {
        std::this_thread::sleep_for(10ms);
    }

    std::this_thread::sleep_for(3000ms);

    while (true) {
        char pressed_key = non_block_getch();

        if (g_show_player_list) {
            // reduce screen flickering
            std::stringstream output_str = {};
            output_str << "\033[2J\033[;HPlayer name\t\tpos\t\tlast pos\tCurrent World : " << curr_bot->GetLocalPtr()->WorldName << "\n";
            output_str << curr_bot->GetLocalPtr()->Name << "\t\t" << curr_bot->GetLocalPtr()->PosX << ", " << curr_bot->GetLocalPtr()->PosY << "\t\t" << curr_bot->GetLocalPtr()->LastPosX << ", " << curr_bot->GetLocalPtr()->LastPosY << "\n";
            for (auto player : *curr_bot->GetPlayerListPtr()) {
                output_str << player.second.Name << "\t\t" << player.second.PosX << ", " << player.second.PosY << "\n";
            }

            std::cout << output_str.str();
        }

        switch (pressed_key)
        {
        case 'W': {
            curr_bot->Move(0, -1);
            break;
        }
        case 'A': {
            curr_bot->Move(-1, 0);
            break;
        }
        case 'S': {
            curr_bot->Move(0, 1);
            break;
        }
        case 'D': {
            curr_bot->Move(1, 0);
            break;
        }
        case 'J': {
            // ubisoft nuked the tordawn312312 world :sad:
            curr_bot->JoinWorld("tordawn3123122");
            break;
        }
        case 'L': {
            curr_bot->JoinWorld("EXIT");
            break;
        }
        case '1': {
            g_output_log = true;
            g_show_player_list = false;
            printf("\033[2J\033[;H");
            break;
        }

        case '2': {
            g_output_log = false;
            g_show_player_list = true;
            printf("\033[2J\033[;H");
            break;
        }

        }
        std::this_thread::sleep_for(100ms);
    }

    return 0;
}