#pragma once

#include <string>
#include <vector>

typedef std::string TextPacket;

enum ePacketType : int32_t {
    NET_MESSAGE_UNKNOWN,
    NET_MESSAGE_SERVER_HELLO,
    NET_MESSAGE_GENERIC_TEXT,
    NET_MESSAGE_GAME_MESSAGE,
    NET_MESSAGE_GAME_PACKET,
    NET_MESSAGE_ERROR,
    NET_MESSAGE_TRACK,
    NET_MESSAGE_CLIENT_LOG_REQUEST,
    NET_MESSAGE_CLIENT_LOG_RESPONSE
};

enum eTankPacketType : uint8_t {
    NET_GAME_PACKET_STATE,
    NET_GAME_PACKET_CALL_FUNCTION,
    NET_GAME_PACKET_UPDATE_STATUS,
    NET_GAME_PACKET_TILE_CHANGE_REQUEST,
    NET_GAME_PACKET_SEND_MAP_DATA,
    NET_GAME_PACKET_SEND_TILE_UPDATE_DATA,
    NET_GAME_PACKET_SEND_TILE_UPDATE_DATA_MULTIPLE,
    NET_GAME_PACKET_TILE_ACTIVATE_REQUEST,
    NET_GAME_PACKET_TILE_APPLY_DAMAGE,
    NET_GAME_PACKET_SEND_INVENTORY_STATE,
    NET_GAME_PACKET_ITEM_ACTIVATE_REQUEST,
    NET_GAME_PACKET_ITEM_ACTIVATE_OBJECT_REQUEST,
    NET_GAME_PACKET_SEND_TILE_TREE_STATE,
    NET_GAME_PACKET_MODIFY_ITEM_INVENTORY,
    NET_GAME_PACKET_ITEM_CHANGE_OBJECT,
    NET_GAME_PACKET_SEND_LOCK,
    NET_GAME_PACKET_SEND_ITEM_DATABASE_DATA,
    NET_GAME_PACKET_SEND_PARTICLE_EFFECT,
    NET_GAME_PACKET_SET_ICON_STATE,
    NET_GAME_PACKET_ITEM_EFFECT,
    NET_GAME_PACKET_SET_CHARACTER_STATE,
    NET_GAME_PACKET_PING_REPLY,
    NET_GAME_PACKET_PING_REQUEST,
    NET_GAME_PACKET_GOT_PUNCHED,
    NET_GAME_PACKET_APP_CHECK_RESPONSE,
    NET_GAME_PACKET_APP_INTEGRITY_FAIL,
    NET_GAME_PACKET_DISCONNECT,
    NET_GAME_PACKET_BATTLE_JOIN,
    NET_GAME_PACKET_BATTLE_EVENT,
    NET_GAME_PACKET_USE_DOOR,
    NET_GAME_PACKET_SEND_PARENTAL,
    NET_GAME_PACKET_GONE_FISHIN,
    NET_GAME_PACKET_STEAM,
    NET_GAME_PACKET_PET_BATTLE,
    NET_GAME_PACKET_NPC,
    NET_GAME_PACKET_SPECIAL,
    NET_GAME_PACKET_SEND_PARTICLE_EFFECT_V2,
    NET_GAME_PACKET_ACTIVE_ARROW_TO_ITEM,
    NET_GAME_PACKET_SELECT_TILE_INDEX,
    NET_GAME_PACKET_SEND_PLAYER_TRIBUTE_DATA
};

struct TankPacketFlag {
    uint32_t bNone : 1;
    uint32_t bUnk : 1;
    uint32_t bResetVisualState : 1;
    uint32_t bExtended : 1;
    uint32_t bRotateLeft : 1;
    uint32_t bOnSolid : 1;
    uint32_t bOnFireDamage : 1;
    uint32_t bOnJump : 1;
    uint32_t bOnKilled : 1;
    uint32_t bOnPunched : 1;
    uint32_t bOnPlaced : 1;
    uint32_t bOnTileAction : 1;
    uint32_t bOnGotPunched : 1;
    uint32_t bOnRespawned : 1;
    uint32_t bOnCollectObject : 1;
    uint32_t bOnTrampoline : 1;
    uint32_t bOnDamage : 1;
    uint32_t bOnSlide : 1;
    uint32_t pad_1 : 3;
    uint32_t bOnWallHang : 1;
    uint32_t pad_2 : 3;
    uint32_t bOnAcidDamage : 1;
    uint32_t pad_3 : 6;
};

#pragma pack(push, 1)
struct TankPacket {
    struct {
        eTankPacketType Type;

        union {
            uint8_t ObjectType = 0;
            uint8_t PunchIndex;
            uint8_t NpcType;
        };
        union {
            uint8_t JumpCount;
            uint8_t BuildRange;
            uint8_t NpcIndex;
            uint8_t LostItemCount;
        };
        union {
            uint8_t AnimationType;
            uint8_t PunchRange;
            uint8_t NpcAction;
            uint8_t ParticleIndex;
            uint8_t SteamEffect;
            uint8_t DiceResult;
            uint8_t GainedItemCount;
            uint8_t FruitCount;
        };
        union {
            int32_t NetId = 0;
            int32_t OwnerId;
            int32_t EffectFlagsCheck;
            int32_t ObjectChangeType;
            int32_t ParticleEmitterId;
        };
        union {
            int32_t Item = 0;
            int32_t ItemNetId;
            int32_t TargetNetId;
            int32_t PingHash;
            int32_t PupilColor;
            int32_t TilesLength;
        };

        TankPacketFlag Flags;

        union {
            float FloatVariable = 0;
            float WaterSpeed;
            float ObjectCount;
        };
        union {
            int32_t MainData = 0;
            int32_t PingItem;
            int32_t ElapsedMs;
            int32_t Delay;
            int32_t TileDamage;
            int32_t ItemId;
            int32_t ItemSpeed;
            int32_t EffectFlags;
            int32_t ObjectId;
            int32_t Hash;
            int32_t VerifyPosition;
            int32_t ClientHackType;
        };
        union {
            float VectorX = 0;
            float PositionX;
            float Acceleration;
            float PunchRangeIn;
            int32_t ParticlePositionX;
        };
        union {
            float VectorY = 0;
            float PositionY;
            float BuildRangeIn;
            float PunchStrength;
            int32_t ParticlePositionY;
        };
        union {
            float VectorX2 = 0;
            float PositionX2;
            float DestinationX;
            float GravityIn;
            float SpeedOut;
            float VelocityX;
            float ParticleVariable;
            int HackTypePrimary;
        };
        union {
            float VectorY2 = 0;
            float PositionY2;
            float DestinationY;
            float SpeedIn;
            float GravityOut;
            float VelocityY;
            float ParticleAltId;
            int HackTypeSecondary;
        };
        union {
            float ParticleRotation = 0;
            float NpcVariable;
        };
        union {
            uint32_t IntX = 0;
            uint32_t TilePositionX;
            uint32_t ItemAltId;
            uint32_t HairColor;
        };
        union
        {
            uint32_t IntY = 0;
            uint32_t TilePositionY;
            uint32_t ItemCount;
            uint32_t EyesColor;
            uint32_t NpcSpeed;
            uint32_t ParticleSizeAlt;
        };
        uint32_t DataLength = 0;
    } Header;

    std::vector<uint8_t> Value{};
};
#pragma pack(pop)
