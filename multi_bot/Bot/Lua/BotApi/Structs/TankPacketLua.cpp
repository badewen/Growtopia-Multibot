#include "TankPacketLua.h"

#include "../../../../Lua/LuaHelper.h"

void TankPacketLua::PushAsTable(lua_State* L, TankPacket& pkt) {
    lua_newtable(L);
    
    LUAHELPER_SET_TABLE(integer, "type", pkt.Header.Type);
    LUAHELPER_SET_TABLE(integer, "object_type", pkt.Header.ObjectType);
    LUAHELPER_SET_TABLE(integer, "count1", pkt.Header.JumpCount);
    LUAHELPER_SET_TABLE(integer, "count2", pkt.Header.GainedItemCount);
    LUAHELPER_SET_TABLE(integer, "netid", pkt.Header.NetId);
    LUAHELPER_SET_TABLE(integer, "item", pkt.Header.Item);
    LUAHELPER_SET_TABLE(integer, "flags", pkt.Header.FlagsValue)
    LUAHELPER_SET_TABLE(number, "float_var", pkt.Header.FloatVariable)
    LUAHELPER_SET_TABLE(integer, "int_data", pkt.Header.MainData)
    LUAHELPER_SET_TABLE(number, "pos_x", pkt.Header.PositionX)
    LUAHELPER_SET_TABLE(number, "pos_y", pkt.Header.PositionY)
    LUAHELPER_SET_TABLE(number, "pos2_x", pkt.Header.PositionX2)
    LUAHELPER_SET_TABLE(number, "pos2_y", pkt.Header.PositionY2);
    LUAHELPER_SET_TABLE(number, "particle_rotation", pkt.Header.ParticleRotation);
    LUAHELPER_SET_TABLE(integer, "int_x", pkt.Header.IntX);
    LUAHELPER_SET_TABLE(integer, "int_y", pkt.Header.IntY);
}

TankPacket TankPacketLua::GetFromTable(lua_State* L) {
    TankPacket pkt{};

    LUAHELPER_GET_TABLE(integer, "type", pkt.Header.Type);
    LUAHELPER_GET_TABLE(integer, "object_type", pkt.Header.ObjectType);
    LUAHELPER_GET_TABLE(integer, "count1", pkt.Header.JumpCount);
    LUAHELPER_GET_TABLE(integer, "count2", pkt.Header.GainedItemCount);
    LUAHELPER_GET_TABLE(integer, "netid", pkt.Header.NetId);
    LUAHELPER_GET_TABLE(integer, "item", pkt.Header.Item);
    LUAHELPER_GET_TABLE(integer, "flags", pkt.Header.FlagsValue);
    LUAHELPER_GET_TABLE(number, "float_var", pkt.Header.FloatVariable);
    LUAHELPER_GET_TABLE(integer, "int_data", pkt.Header.MainData);
    LUAHELPER_GET_TABLE(number, "pos_x", pkt.Header.PositionX);
    LUAHELPER_GET_TABLE(number, "pos_y", pkt.Header.PositionY);
    LUAHELPER_GET_TABLE(number, "pos2_x", pkt.Header.PositionX2);
    LUAHELPER_GET_TABLE(number, "pos2_y", pkt.Header.PositionY2);
    LUAHELPER_GET_TABLE(number, "particle_rotation", pkt.Header.ParticleRotation);
    LUAHELPER_GET_TABLE(integer, "int_x", pkt.Header.IntX);
    LUAHELPER_GET_TABLE(integer, "int_y", pkt.Header.IntY);

    return pkt;
}
