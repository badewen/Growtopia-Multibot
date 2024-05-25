#pragma once

#include "../../../Lua/ILuaLibrary.h"

#include <string>

#include <proton/Variant.h>

class Bot;
class Packet;
struct TankPacket;
enum ePacketType;
typedef std::string TextPacket;

#define LUABOTAPILIB_BOT_REG_UUID "b8724c6a-d97a"
#define LUABOTAPILIB_TEXTPKT_CB_REG_UUID "55c01d3b-9491"
#define LUABOTAPILIB_TANKPKT_CB_REG_UUID "cc9cd67a-4230"
#define LUABOTAPILIB_VARLISTPKT_CB_REG_UUID "f7d62d8d-403c"
#define LUABOTAPILIB_ONPKT_INVOKER_REG_UUID "fabfc25e-25fa"

class LuaBotApiLib : public ILuaLibrary
{
public:
    LuaBotApiLib(Bot* bot) :
        m_bot{ bot },
        m_text_pkt_cb_thread_state{},
        m_tank_pkt_cb_thread_state{},
        m_var_pkt_cb_thread_state{}
    {
    }

    void OnLoadLibrary(lua_State* L) override;
    void OnUnloadLibrary(lua_State* L) override;

    void TryInvokeOnTextPktCallback(lua_State* L, ePacketType pkt_type, TextPacket pkt);
    void TryInvokeOnTankPktCallback(lua_State* L, TankPacket pkt);
    void TryInvokeOnVarlistPktCallback(lua_State* L, TankPacket pkt, VariantList varlist);

public:
    // SendTextPacket(type, text_pkt);
    static int l_send_text_packet(lua_State* L);

    // SendRawPacket(raw_pkt);
    static int l_send_raw_packet(lua_State* L);

    // GetLocal();
    // returns PlayerLocal table.
    static int l_get_local(lua_State* L);

    // IsInWorld();
    static int l_is_in_world(lua_State* L);

    // IsInGame();
    static int l_is_in_game(lua_State* L);

    // AddHook(string type, string hook_id, function hook);
    /* TYPES                HOOK SIGNATURE
     - "OnTextPacket"       (int packet_type, string packet)
     - "OnTankPacket"       (TankPacket pkt)
     - "OnVarlistPacket"    (TankPacket pkt, Varlist varlist)
    */
    static int l_add_hook(lua_State* L);

    // only meant for debugging purposes.
    // returns { int id, int extra_tile_type }
    static int l_debug_get_faulty_tile(lua_State* L);

    // get all of the dropped object
    // returns table of WorldObject
    static int l_get_objects(lua_State* L);

private:
    Bot* m_bot;

    lua_State* m_text_pkt_cb_thread_state;
    int m_text_pkt_cb_thread_refid{};

    lua_State* m_tank_pkt_cb_thread_state;
    int m_tank_pkt_cb_thread_refid{};

    lua_State* m_var_pkt_cb_thread_state;
    int m_var_pkt_cb_thread_refid{};
};

