#pragma once

#include <vector>
#include <string>

// Credits : https://github.com/playingoDEERUX/growbrewproxy/blob/master/GrowbrewProxy/WorldAndPlayer.cs
// Pasted from https://github.com/badewen/GTProxy/blob/develop/src/world/extra_tile_data.h lol

enum eWorldTileExtraType : uint8_t {
    None = 0,
    Door = 1,
    Sign = 2,
    Lock = 3,
    Seed = 4,
    Dice = 8,
    Provider = 9,
    AchievementBlock = 10,
    HeartMonitor = 11,
    Mannequin = 14,
    GameGrave = 16,
    GameGenerator = 17,
    Xenonite = 18,
    PhoneBooth = 19,
    Spotlight = 22,
    DisplayBlock = 23,
    VendingMachine = 24,
    FishTankPort = 25,
    Forge = 27,
    GivingTree = 28,
    SewingMachine = 32,
    CountryFlag = 33,
    LobsterTrap = 34,
    PaintingEasel = 35,
    WeatherMachine = 40,
    DataBedrock = 42,
    Shelf = 43,
    VipEntrance = 44,
    ChallengeTimer = 45,
    Portrait = 48,
    GuildWeatherMachine = 49,
    DnaExtractor = 51,
    Howler = 52,
    StorageBlock = 54,
    CookingOven = 55,
    AudioRack = 56,
    AdventureBegin = 58,
    TombRobber = 59,
    TrainingPort = 61,
    GuildItem = 65,
    KrakenGalaticBlock = 80,
    FriendsEntrance = 81,
};

struct WorldTileDoorExtra {
    std::string Label;
    uint8_t Unk1;
};

struct WorldTileSignExtra {
    std::string Label;
};

struct WorldTileLockExtra {
    uint8_t Flags;
    uint8_t MinimumLevel;
    uint32_t OwnerId;
    uint8_t UnkArr[7];
    std::vector<uint32_t> AccessList;
};

struct WorldTileSeedExtra {
    uint32_t TimePassed;
    uint8_t FruitCount;
};

struct WorldTileDiceExtra {
    uint8_t Pattern;
};

struct WorldTileProviderExtra {
    uint32_t TimePassed;
};

struct WorldTileAchievementBlockExtra {
    uint32_t UserId;
    uint8_t Achievement;
};

struct WorldTileHeartMonitorExtra {
    std::string Name;
    uint32_t UserId;
};

struct WorldTileMannequinExtra {
    std::string Label;
    uint8_t Unk1;
    uint16_t Unk2;
    uint16_t Unk3;
    uint16_t Hat;
    uint16_t Shirt;
    uint16_t Pants;
    uint16_t Boots;
    uint16_t Face;
    uint16_t Hand;
    uint16_t Back;
    uint16_t Hair;
    uint16_t Neck;
};

struct WorldTileGameGraveExtra {
    uint8_t Team;
};

struct WorldTileGameGeneratorExtra {};

struct WorldTileXenoniteExtra {
    uint8_t Unk1[5];
};

struct WorldTilePhoneBoothExtra {
    uint16_t Hat;
    uint16_t Shirt;
    uint16_t Pants;
    uint16_t Shoes;
    uint16_t Face;
    uint16_t Hand;
    uint16_t Back;
    uint16_t Hair;
    uint16_t Neck;
};

struct WorldTileSpotlightExtra {};

struct WorldTileDisplayBlockExtra {
    uint32_t ItemId;
};

struct WorldTileVendingMachineExtra {
    uint32_t ItemId;
    int32_t Price;
};

struct WorldTileFishTankPortExtra {
    uint8_t Flags;
    struct FishInfo {
        uint32_t FishItemId, Lbs;
    };
    std::vector<FishInfo> Fishes;
};

struct WorldTileForgeExtra {
    uint32_t Temperature;
};

struct WorldTileGivingTreeExtra {
    uint8_t Harvested;
    uint8_t DecorationPercent;
    uint32_t Unk1;
};

struct WorldTileSewingMachineExtra {
    std::vector<uint32_t> BoltIdList;
};

struct WorldTileCountryFlagExtra {
    std::string Country;
};

struct WorldTileLobsterTrapExtra {};

struct WorldTilePaintingEaselExtra {
    uint32_t ItemId;
    std::string Label;
};

struct WorldTileWeatherMachineExtra {
    // spinning, invert, etc idk
    uint32_t Flags;
};

struct WorldTileDataBedrockExtra {
    uint8_t Unk1[21];
};

struct WorldTileShelfExtra {
    uint32_t TopLeft, TopRight;
    uint32_t BottomLeft, BottomRight;
};

struct WorldTileVipEntranceExtra {
    uint8_t Unk1;
    uint32_t OwnerId;
    std::vector<uint32_t> AllowedIdList;
};

struct WorldTileChallengeTimerExtra {};

struct WorldTilePortraitExtra {
    std::string Label;
    uint32_t Unk1, Unk2, Unk3, Unk4;
    uint32_t Face, Hat, Hair;
    // Eyecolor and eye??
    uint16_t Unk5, Unk6;
};

struct WorldTileGuildWeatherMachineExtra {
    uint32_t Unk1;
    uint32_t Gravity;
    uint8_t Flags;
};

struct WorldTileDnaExtractorExtra {};

struct WorldTileHowlerExtra {};

struct WorldTileStorageBlockExtra {
    struct ItemInfo {
        uint32_t ItemId, ItemAmount;
    };
    std::vector<ItemInfo> Items;
};

struct WorldTileCookingOvenExtra {};

struct WorldTileAudioRackExtra {
    std::string Note;
    uint32_t Volume;
};

struct WorldTileAdventureBeginsExtra {};

struct WorldTileTombRobberExtra {};

struct WorldTileTrainingPortExtra {
    uint32_t FishLb;
    uint16_t FishStatus;
    uint32_t FishId;
    uint32_t FishTotalExp;
    uint8_t Unk1[8];
    uint32_t FishLevel;
    uint32_t Unk2;
    uint8_t Unk3[5];
};

struct WorldTileGuildItemExtra {
    uint8_t Unk1[17];
};

struct WorldTileKrakenBlockExtra {
    uint8_t PatternIndex;
    uint32_t Unk1;
    uint8_t R, G, B;
};

struct WorldTileFriendsEntranceExtra {
    uint32_t OwnerUserId;
    uint16_t Unk1;
    std::vector<uint32_t> AllowedUserList;
};