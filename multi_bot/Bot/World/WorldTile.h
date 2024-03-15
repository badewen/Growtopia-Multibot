#pragma once

#include "WorldTileExtraTypes.h"
#include <memory>

// Pasted from https://github.com/badewen/GTProxy/blob/develop/src/world/world_tile.h

struct WorldTileFlag {
    uint16_t bTileExtra  : 1;
    uint16_t bLocked     : 1;
    uint16_t bUnk1       : 2;
    uint16_t bSeed       : 1;
    uint16_t bFlipped    : 1;
    uint16_t bOpen       : 1;
    uint16_t bPublic     : 1;
    uint16_t bUnk2       : 1;
    uint16_t bSilenced   : 1;
    uint16_t bWater      : 1;
    uint16_t bUnk3       : 1;
    uint16_t bFire       : 1;
    uint16_t bRed        : 1;
    uint16_t bBlue       : 1;
    uint16_t bGreen      : 1;
};

union WorldTileExtraAllUnion {
    WorldTileDoorExtra DoorExtra;
    WorldTileSignExtra SignExtra;
    WorldTileLockExtra LockExtra;
    WorldTileSeedExtra SeedExtra;
    WorldTileDiceExtra DiceExtra;
    WorldTileProviderExtra ProviderExtra;
    WorldTileAchievementBlockExtra AchievementBlockExtra;
    WorldTileHeartMonitorExtra HeartMonitorExtra;
    WorldTileMannequinExtra MannequinExtra;
    WorldTileGameGraveExtra GameGraveExtra;
    WorldTileGameGeneratorExtra GameGeneratorExtra;
    WorldTileXenoniteExtra XenoniteExtra;
    WorldTilePhoneBoothExtra PhoneBoothExtra;
    WorldTileSpotlightExtra SpotlightExtra;
    WorldTileDisplayBlockExtra DisplayBlockExtra;
    WorldTileVendingMachineExtra VendingMachineExtra;
    WorldTileFishTankPortExtra FishTankPortExtra;
    WorldTileForgeExtra ForgeExtra;
    WorldTileGivingTreeExtra GivingTreeExtra;
    WorldTileSewingMachineExtra SewingMachineExtra;
    WorldTileCountryFlagExtra CountryFlagExtra;
    WorldTileLobsterTrapExtra LobsterTrapExtra;
    WorldTilePaintingEaselExtra PaintingEaselExtra;
    WorldTileWeatherMachineExtra WeatherMachineExtra;
    WorldTileDataBedrockExtra DataBedrockExtra;
    WorldTileShelfExtra ShelfExtra;
    WorldTileVipEntranceExtra VipEntranceExtra;
    WorldTileChallengeTimerExtra ChallengeTimerExtra;
    WorldTilePortraitExtra PortraitExtra;
    WorldTileGuildWeatherMachineExtra GuildWeatherMachineExtra;
    WorldTileDnaExtractorExtra DnaExtractorExtra;
    WorldTileHowlerExtra HowlerExtra;
    WorldTileStorageBlockExtra StorageBlockExtra;
    WorldTileCookingOvenExtra CookingOvenExtra;
    WorldTileAudioRackExtra AudioRackExtra;
    WorldTileAdventureBeginsExtra AdventureBeginsExtra;
    WorldTileTombRobberExtra TombRobberExtra;
    WorldTileTrainingPortExtra TrainingPortExtra;
    WorldTileGuildItemExtra GuildItemExtra;
    WorldTileKrakenBlockExtra KrakenBlockExtra;
    WorldTileFriendsEntranceExtra FriendEntranceExtra;
    WorldTileCrystalExtra CrystalExtra;
    WorldTileSolarCollectorExtra SolarCollectorExtra;
    WorldTileSteamEngineExtra SteamEngineExtra;
    WorldTileGeigerChargerExtra GeigerChargerExtra;
    WorldTileItemSuckerExtra ItemSuckerExtra;
    WorldTileGrowscanExtra GrowscanExtra;
    WorldTileTemporaryPlatformExtra TemporaryPlatformExtra;
    WorldTileSafeVaultExtra SafeVaultExtra;
    WorldTileInfinityWeatherMachineExtra InfinityWeatherMachineExtra;
};

class WorldTile {
public:
    WorldTile() {
        ExtraTileData._struct_buff_ptr = m_struct_buffer;
    }

public:
    uint16_t Fg{}, Bg{};
    // for fish tank, lock, etc.
    uint16_t ParentTileIndex{};

    WorldTileFlag Flags{};

    // if bLocked is set, this contains the index to the locking block.
    uint16_t LockIndex{};

    // if bTileExtra is set.
    eWorldTileExtraType ExtraTileDataType{};

    // assigning a dynamically allocated pointer will be taken over by this class.
    // NOTE : USE MALLOC.
    // the pointer is managed by this class. 
    // freeing this pointer outside of the class lifetime may
    // result in UB.
    union {
        // blame c++ constructor and destructor non-sense.
        uint8_t* _struct_buff_ptr;
        WorldTileDoorExtra* DoorExtra;
        WorldTileSignExtra* SignExtra;
        WorldTileLockExtra* LockExtra;
        WorldTileSeedExtra* SeedExtra;
        WorldTileDiceExtra* DiceExtra;
        WorldTileProviderExtra* ProviderExtra;
        WorldTileAchievementBlockExtra* AchievementBlockExtra;
        WorldTileHeartMonitorExtra* HeartMonitorExtra;
        WorldTileMannequinExtra* MannequinExtra;
        WorldTileGameGraveExtra* GameGraveExtra;
        WorldTileGameGeneratorExtra* GameGeneratorExtra;
        WorldTileXenoniteExtra* XenoniteExtra;
        WorldTilePhoneBoothExtra* PhoneBoothExtra;
        WorldTileSpotlightExtra* SpotlightExtra;
        WorldTileDisplayBlockExtra* DisplayBlockExtra;
        WorldTileVendingMachineExtra* VendingMachineExtra;
        WorldTileFishTankPortExtra* FishTankPortExtra;
        WorldTileForgeExtra* ForgeExtra;
        WorldTileGivingTreeExtra* GivingTreeExtra;
        WorldTileSewingMachineExtra* SewingMachineExtra;
        WorldTileCountryFlagExtra* CountryFlagExtra;
        WorldTileLobsterTrapExtra* LobsterTrapExtra;
        WorldTilePaintingEaselExtra* PaintingEaselExtra;
        WorldTileWeatherMachineExtra* WeatherMachineExtra;
        WorldTileDataBedrockExtra* DataBedrockExtra;
        WorldTileShelfExtra* ShelfExtra;
        WorldTileVipEntranceExtra* VipEntranceExtra;
        WorldTileChallengeTimerExtra* ChallengeTimerExtra;
        WorldTilePortraitExtra* PortraitExtra;
        WorldTileGuildWeatherMachineExtra* GuildWeatherMachineExtra;
        WorldTileDnaExtractorExtra* DnaExtractorExtra;
        WorldTileHowlerExtra* HowlerExtra;
        WorldTileStorageBlockExtra* StorageBlockExtra;
        WorldTileCookingOvenExtra* CookingOvenExtra;
        WorldTileAudioRackExtra* AudioRackExtra;
        WorldTileAdventureBeginsExtra* AdventureBeginsExtra;
        WorldTileTombRobberExtra* TombRobberExtra;
        WorldTileTrainingPortExtra* TrainingPortExtra;
        WorldTileGuildItemExtra* GuildItemExtra;
        WorldTileKrakenBlockExtra* KrakenBlockExtra;
        WorldTileFriendsEntranceExtra* FriendEntranceExtra;
        WorldTileCrystalExtra* CrystalExtra;
        WorldTileSolarCollectorExtra* SolarCollectorExtra;
        WorldTileSteamEngineExtra* SteamEngineExtra;
        WorldTileGeigerChargerExtra* GeigerChargerExtra;
        WorldTileItemSuckerExtra* ItemSuckerExtra;
        WorldTileGrowscanExtra* GrowscanExtra;
        WorldTileTemporaryPlatformExtra* TemporaryPlatformExtra;
        WorldTileSafeVaultExtra* SafeVaultExtra;
        WorldTileInfinityWeatherMachineExtra* InfinityWeatherMachineExtra;
    } ExtraTileData{};

private:
    uint8_t m_struct_buffer[sizeof(WorldTileExtraAllUnion)]{};
};