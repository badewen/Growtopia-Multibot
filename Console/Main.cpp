#include <memory>
#include <chrono>

#define ESC_SEQ_CLEAR_SCREEN "\033[2J\033[;H"

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
bool g_show_inventory = false;

class loger : public ILogger
{
protected:
    void ILogger::output_log(ILogger::LogType type, std::string log) {
        if (!g_output_log) { return; }
        switch (type)
        {
        case ILogger::LogType::Debug: {
            spdlog::debug(log);
            break;
        }
        case ILogger::LogType::Info: {
            spdlog::info(log);
            break;
        }
        case ILogger::LogType::Warn: {
            spdlog::warn(log);
            break;
        }

        case ILogger::LogType::Error: {
            spdlog::error(log);
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

    std::shared_ptr<ItemDatabase> items_dat = std::make_shared<ItemDatabase>();

    spdlog::info("Parsing items.dat");

    if (!items_dat->LoadFromFile("items.dat")) {
        spdlog::error("Cant find items.dat. Terminating...");
        return EXIT_FAILURE;
    }

    spdlog::info("Items.dat parsed. Lmao memory usage spiked");

    BotManager bot_mgr {};
    bot_mgr.SetItemDatabase(items_dat);
    std::shared_ptr<Bot> curr_bot = bot_mgr.AddBot("test_bot", std::make_shared<loger>());

    if (!curr_bot->CreateHost()) {
        spdlog::info("Failed to create host");
    }

    //curr_bot->SetLoginGuest("MAC", "RID");
    //curr_bot->GenerateNewSpoof();
    //curr_bot->SetLoginGrowID("GROWID", "PASS");

    if (!curr_bot->ConnectWithHttp()) {
        spdlog::error("Failed to connect. Press \"R\" to try connecting again.");
    }

    curr_bot->StartBotThread();

    while (true) {
        char pressed_key = non_block_getch();

        if (g_show_player_list) {
            // reduce screen flickering
            std::stringstream output_str = {};
            std::cout << ESC_SEQ_CLEAR_SCREEN;

            output_str << "Player name\t\tpos\t\tlast pos\tCurrent World : " << curr_bot->GetLocalPtr()->WorldName << "\n";
            output_str << curr_bot->GetLocalPtr()->Name << "\t\t" << curr_bot->GetLocalPtr()->PosX << ", " << curr_bot->GetLocalPtr()->PosY << "\t\t" << curr_bot->GetLocalPtr()->LastPosX << ", " << curr_bot->GetLocalPtr()->LastPosY << "\n";
            for (auto player : *curr_bot->GetPlayerListPtr()) {
                output_str << player.second.Name << "\t\t" << player.second.PosX << ", " << player.second.PosY << "\n";
            }

            std::cout << output_str.str();
        }

        if (g_show_inventory) {
            std::stringstream output_str = {};
            std::cout << ESC_SEQ_CLEAR_SCREEN;

            output_str << "Inventory Info\nCount\t\tItem ID\t\t\t(" << curr_bot->GetInventoryRef().GetAvailInvSpace() << "/" << curr_bot->GetInventoryRef().GetTotalInvSpace() << ")\n";

            auto item_list = curr_bot->GetInventoryRef().GetInventory();

            for (const auto& item : item_list) {
                output_str << item.second << "x\t" << item.first << "\n";
            }

            std::cout << output_str.str();
        }

        if (pressed_key != NULL) {
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
                curr_bot->JoinWorld("tordawn3123132");
                break;
            }
            case 'L': {
                curr_bot->JoinWorld("EXIT");
                break;
            }
            case 'P': {
                curr_bot->Place(2, -1, 0);
                break; 
            }
            case 'R': {
                spdlog::info("Connecting to the server...");
                if (!curr_bot->ConnectWithHttp()) {
                    spdlog::error("Failed to connect. Press \"R\" to try connecting again.");
                }
                break;
            }
            case '1': {
                g_output_log = true;
                g_show_player_list = false;
                g_show_inventory = false;
                printf(ESC_SEQ_CLEAR_SCREEN);
                break;
            }

            case '2': {
                g_output_log = false;
                g_show_player_list = true;
                g_show_inventory = false;
                printf(ESC_SEQ_CLEAR_SCREEN);
                break;
            }         
            
            case '3': {
                g_output_log = false;
                g_show_player_list = false;
                g_show_inventory = true;
                printf(ESC_SEQ_CLEAR_SCREEN);
                break;
            }

            default: {
                spdlog::info("KEY PRESS {}", pressed_key);
                break;
            }
            }
        }
        std::this_thread::sleep_for(100ms);
    }

    return 0;
}