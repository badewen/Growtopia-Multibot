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
#include <Utils/Random.h>

#include <Utils/Timer.h>

// this project is more of a place to test the curr_bot->

using namespace std::chrono_literals;

bool g_output_log = true;

bool g_show_player_list = false;
bool g_show_inventory = false;
bool g_show_npc = false;
bool g_show_world_debug_progg = false;

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
            output_str << curr_bot->GetLocalPtr()->Name << "\t\t" << curr_bot->GetLocalPtr()->PosX << ", " << curr_bot->GetLocalPtr()->PosY << "\t\t" << curr_bot->GetLocalPtr()->LastPosX << ", " << curr_bot->GetLocalPtr()->LastPosY << "\t\t" << *(uint32_t*)&curr_bot->GetLocalPtr()->Gems << "\n";
            for (auto player : *curr_bot->GetPlayerListPtr()) {
                output_str << player.second.Name << "\t\t" << player.second.PosX << ", " << player.second.PosY << "\t\t" << player.second.LastPosX << ", " << player.second.LastPosY << "\t\t" << *(uint32_t*)&player.second.Flags << "\n";
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

        if (g_show_npc) {
            std::stringstream output_str = {};
            std::cout << ESC_SEQ_CLEAR_SCREEN;

            output_str << "Type\t\tIndex\t\tPos\t\t\t\tTarget Pos\n";

            // when you want to calculate the current npc pos.
            curr_bot->GetCurrentWorldRef().UpdateNPCsPos();

            auto npc_list = curr_bot->GetCurrentWorldRef().GetNPCs();

            for (const auto& npc : npc_list) {
                output_str << std::to_string(npc.Type) << "\t\t"
                           << std::to_string(npc.Index) << "\t\t("
                           << npc.PosX << ", " << npc.PosY << ")\t("
                           << npc.TargetX << ", " << npc.TargetY << ")\n";
            }

            std::cout << output_str.str();
        }

        if (g_show_world_debug_progg) {
            LuaExecutorError err = curr_bot->ExecuteScript(R"(
-- WORLD CHECKER
require "math"

world_lett_limit = 3
chars = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z' }

failed_enter_world = false

function joinworld(name)
	SendTextPacket(3, "action|join_request\nname|"..name.."\ninvitedWorld|0")
end


AddHook("OnVarlist", "failed_world_hook", function(pkt, var)
	if (var[0] == "OnFailedToEnterWorld") then
		failed_enter_world = true
	end
end)
SendWebhook("YOUR_WEBHOOK_URL", 
			'{"content": "HELLO WORLD"}')
for i=0,((#chars)^world_lett_limit)-1 do
	world_name = ""

	for j=0,world_lett_limit - 1 do
		if i < (#chars)^j then
			break
		end
		world_name = world_name .. chars[ (math.floor((i/(#chars)^j)) % #chars) + 1]
	end

	failed_enter_world = false

	print("Joining world "..world_name)

	joinworld(world_name)

	while (not IsInWorld()) do
		if (failed_enter_world) then
			goto end_loop
		end
		Sleep(100)
	end

	if (next(DebugGetFaultyTile()) ~= nil) then
		SendWebhook("YOUR_WEBHOOK_URL", 
			'{"content": "World parser failed "' .. i .. "th world ".. world_name .. ". Data " .. DebugGetFaultyTile().id .. ", " .. DebugGetFaultyTile().extra_tile_type .. '. "}')
	end

	Sleep(1500)

	::end_loop::
end
)");
            spdlog::info("Script returned {}, {}", err.ErrorCode, err.ErrorMessage);
            spdlog::info("WORLD TESTING IS DONE");
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
                curr_bot->JoinWorld("A");
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
                spdlog::info("Reconnecting to the server...");
                curr_bot->Disconnect();
                std::this_thread::sleep_for(30ms);
                if (!curr_bot->ConnectWithHttp()) {
                    spdlog::error("Failed to connect. Press \"R\" to try connecting again.");
                }
                break;
            }
            case 'T': {
                spdlog::info("Disconnecting from the server");
                curr_bot->SendPacket({ ePacketType::NET_MESSAGE_GAME_MESSAGE, "action|quit" });
                curr_bot->DisconnectNow();
                break;
            }
            case 'X': {
                spdlog::info("Executing test script..");
                std::thread th{ [&]() {
                LuaExecutorError err = curr_bot->ExecuteScript(R"(   
-- text packet test
SendTextPacket(2, "action|input\n|text|HELLO")

Sleep(3000)

-- raw packet and get local test 
pkt = {}
pkt.type = 3
pkt.int_data = 2
pkt.pos_x = GetLocal().pos_x
pkt.pos_y = GetLocal().pos_y
pkt.int_x = (GetLocal().pos_x // 32) + 1
pkt.int_y = (GetLocal().pos_y // 32) + 1
SendRawPacket(pkt)
)");
                spdlog::info("Script returned {}, {}", err.ErrorCode, err.ErrorMessage);
                } };
                th.detach();
                
                break;
            }
            case '1': {
                g_output_log = true;
                g_show_player_list = false;
                g_show_inventory = false;
                g_show_npc = false;
                g_show_world_debug_progg = false;
                printf(ESC_SEQ_CLEAR_SCREEN);
                break;
            }

            case '2': {
                g_output_log = false;
                g_show_player_list = true;
                g_show_inventory = false;
                g_show_npc = false;
                g_show_world_debug_progg = false;
                printf(ESC_SEQ_CLEAR_SCREEN);
                break;
            }         
            
            case '3': {
                g_output_log = false;
                g_show_player_list = false;
                g_show_inventory = true;
                g_show_npc = false;
                g_show_world_debug_progg = false;
                printf(ESC_SEQ_CLEAR_SCREEN);
                break;
            }

            case '4': {
                g_output_log = false;
                g_show_player_list = false;
                g_show_inventory = false;
                g_show_npc = true;
                g_show_world_debug_progg = false;
                printf(ESC_SEQ_CLEAR_SCREEN);
                break;
            }

            // serialize tons of worlds
            case 'Z': {
                g_output_log = false;
                g_show_player_list = false;
                g_show_inventory = false;
                g_show_npc = false;
                g_show_world_debug_progg = true;
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