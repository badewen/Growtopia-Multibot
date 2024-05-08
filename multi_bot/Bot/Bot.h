#pragma once

#include <string>
#include <thread>
#include <memory>

// need to be here smh
#include "../Network/Http/HttpClient.h"

#include <enet/enet.h>
#include <proton/Variant.h>

#include "../Logger/ILogger.hpp"
#include "../Packet/PacketTypes.h"
#include "../Network/Enet/EnetClient.h"
#include "../ItemDatabase/ItemDatabase.h"
#include "../Lua/LuaExecutor.h"
#include "PacketHandler/PacketHandlerManager.h"
#include "Inventory/Inventory.h"
#include "World/WorldInfo.h"
#include "NetAvatar.h"
#include "RedirectServerData.h"
#include "LoginData.h"

#include "../Lua/BuiltinLibs/BuiltinLuaLib.h"
#include "Lua/BotApi/LuaBotApiLib.h"

class Bot : public EnetClient
{
public:
    Bot(std::shared_ptr<ILogger> logger, std::shared_ptr<ItemDatabase> item_database)
        : EnetClient::EnetClient{ logger },
        m_logger{ logger },
        m_item_database{ std::move(item_database) },
        m_lua_executor{}
    {
        GenerateNewSpoof();
        m_current_world = { m_item_database };
        m_default_packet_handler_registry = std::make_shared<PacketHandlerRegistry>( this );
        m_packet_handler_dispatcher.SetHandlerRegistry(m_default_packet_handler_registry);
        m_lua_bot_api_lib = std::make_shared<LuaBotApiLib>(this);
        m_lua_executor.AddLibrary(std::make_shared<BuiltinLuaLib>());
        m_lua_executor.AddLibrary(m_lua_bot_api_lib);
    }

        
    void __nothrow GenerateNewSpoof();

    void SetLoginGuest(std::string mac, std::string rid);
    void SetLoginGrowID(std::string growid, std::string password);

    bool ConnectWithHttp();
    bool Connect(std::string ip, std::string port, std::string meta, bool use_new_packet);

    void StartBotThread();
    void StopBotThread();

    void JoinWorld(std::string world);
    void Move(float x, float y);
    // if off_x < 0 then it is on left side
    // if off_y < 0 then it is on the upper side
    void Place(uint32_t item_id, int32_t off_x, int32_t off_y);
    void Punch(int32_t off_x, int32_t off_y);

    // true if there is no syntax error, false otherwise
    LuaExecutorError ExecuteScript(std::string script);

    bool IsInGame() { return m_is_in_game; }
    bool IsInWorld() { return m_is_in_world; }

    std::unordered_map<int32_t, NetAvatar>* GetPlayerListPtr() { return &m_player_list; }
    NetAvatar* GetLocalPtr() { return &m_local; }
    Inventory& GetInventoryRef() { return m_inventory; }
    WorldInfo& GetCurrentWorldRef() { return m_current_world; }
    HttpClient& GetCurrentHttpClientRef() { return m_http_cl; } // idk maybe implement login bypass on the app's code side?
    std::shared_ptr<ILogger> GetLoggerPtr() { return m_logger; }
    std::shared_ptr<ItemDatabase> GetItemDatabasePtr() { return m_item_database; }
    RedirectServerData GetRedirectData() { return m_redirect_server_data; }
    LoginData GetLoginData() { return m_login_data; }

    void SetIsInWorld(bool in_world) { m_is_in_world = in_world; }
    void SetIsInGame(bool in_game) { m_is_in_game = in_game; }
    void SetIsRedirected(bool is_redirected) { m_is_redirected = is_redirected; }
    void SetShouldReconnect(bool should_reconnect) { m_reconnect = should_reconnect; }
    void SetHttpProxySocks5(std::string ip, std::string username, std::string pass) { m_http_cl.SetSocks5(ip, username, pass); }
    void SetENetProxySocks5(std::string ip, std::string username, std::string pass) { EnetClient::SetSocks5Url(ip, username, pass); }

    void SetRedirectData(const RedirectServerData* server_data);
    void SetServerIp(std::string ip) { m_server_ip = ip; }
    void SetServerPort(std::string port) { m_server_port = port; }

private:
    void bot_thread();

    void on_connect() override;
    void on_receive(ENetPacket* pkt) override;
    void on_disconnect() override;

    void on_incoming_text_packet(ePacketType type, TextPacket pkt);
    void on_incoming_tank_packet(TankPacket pkt);
    void on_incoming_varlist(VariantList varlist, TankPacket pkt);
private:
    LoginData m_login_data{};

    bool m_is_in_world = false, m_is_in_game = false;
    bool m_is_redirected = false, m_reconnect = false;

    bool m_is_running = false, m_using_new_packet = false;

    std::string m_server_ip{}, m_server_port{};

    RedirectServerData m_redirect_server_data{};

    std::thread m_bot_thread{};

    NetAvatar m_local{};
    // netid, netavatar
    std::unordered_map<int32_t, NetAvatar> m_player_list{};

    Inventory m_inventory{};
    WorldInfo m_current_world{ nullptr };

    bool m_is_bot_moving = false;

    std::shared_ptr<ItemDatabase> m_item_database{};

    LuaExecutor m_lua_executor{};
    PacketHandlerManager m_packet_handler_dispatcher{};
    std::shared_ptr<PacketHandlerRegistry> m_default_packet_handler_registry{};

    std::shared_ptr<LuaBotApiLib> m_lua_bot_api_lib{};

    std::shared_ptr<ILogger> m_logger{};
    HttpClient m_http_cl { "https://www.growtopia1.com" };
};

