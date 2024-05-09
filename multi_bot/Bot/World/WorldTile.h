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

class WorldTile {
public:
    WorldTile() = default;

    // i think this is needed because the previous code is leaking, but unnoticeable.
    void InitializeTileExtra(eWorldTileExtraType type);
    void FreeTileExtra();

public:
    uint16_t Fg{}, Bg{};
    // for fish tank, lock, etc.
    uint16_t ParentTileIndex{};

    WorldTileFlag Flags{};

    // if bLocked is set, this contains the index to the locking block.
    uint16_t LockIndex{};

    // if bTileExtra is set.
    eWorldTileExtraType ExtraTileDataType{};

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
        WorldTileMagicEggExtra* MagicEggExtra;
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
        WorldTileSteamOrganExtra* SteamOrganExtra;
        WorldTileSilkWormExtra* SilkWormExtra;
        WorldTileSewingMachineExtra* SewingMachineExtra;
        WorldTileCountryFlagExtra* CountryFlagExtra;
        WorldTileLobsterTrapExtra* LobsterTrapExtra;
        WorldTilePaintingEaselExtra* PaintingEaselExtra;
        WorldTilePetBattleCageExtra* PetBattleCageExtra;
        WorldTilePetTrainerExtra* PetTrainerExtra;
        WorldTileWeatherMachineExtra* WeatherMachineExtra;
        WorldTileSpiritStorageUnitExtra* SpiritStorageUnitExtra;
        WorldTileDataBedrockExtra* DataBedrockExtra;
        WorldTileShelfExtra* ShelfExtra;
        WorldTileVipEntranceExtra* VipEntranceExtra;
        WorldTileChallengeTimerExtra* ChallengeTimerExtra;
        WorldTileFishWallMountExtra* FishWallMountExtra;
        WorldTilePortraitExtra* PortraitExtra;
        WorldTileGuildWeatherMachineExtra* GuildWeatherMachineExtra;
        WorldTileFossilPrepStationExtra* FossilPrepStationExtra;
        WorldTileDnaExtractorExtra* DnaExtractorExtra;
        WorldTileHowlerExtra* HowlerExtra;
        WorldTileChemsynthTankExtra* ChemsynthTankExtra;
        WorldTileStorageBlockExtra* StorageBlockExtra;
        WorldTileCookingOvenExtra* CookingOvenExtra;
        WorldTileAudioRackExtra* AudioRackExtra;
        WorldTileAdventureBeginsExtra* AdventureBeginsExtra;
        WorldTileTombRobberExtra* TombRobberExtra;
        WorldTileBalloonOMaticExtra* BalloonOMaticExtra;
        WorldTileTrainingPortExtra* TrainingPortExtra;
        WorldTileGuildItemExtra* GuildItemExtra;
        WorldTileKrakenGalaticBlockExtra* KrakenGalaticBlockExtra;
        WorldTileFriendsEntranceExtra* FriendsEntranceExtra;
        WorldTileCrystalExtra* CrystalExtra;
        WorldTileCrimeInProgressExtra* CrimeInProgressExtra;
        WorldTileSolarCollectorExtra* SolarCollectorExtra;
        WorldTileSteamEngineExtra* SteamEngineExtra;
        WorldTileLockBotExtra* LockBotExtra;
        WorldTileGeigerChargerExtra* GeigerChargerExtra;
        WorldTileItemSuckerExtra* ItemSuckerExtra;
        WorldTileCyBotExtra* CyBotExtra;
        WorldTileGrowscanExtra* GrowscanExtra;
        WorldTileContainmentFieldPowerNodeExtra* ContainmentFieldPowerNodeExtra;
        WorldTileSpiritBoardExtra* SpiritBoardExtra;
        WorldTileStormyCloudExtra* StormyCloudExtra;
        WorldTileTemporaryPlatformExtra* TemporaryPlatformExtra;
        WorldTileSafeVaultExtra* SafeVaultExtra;
        WorldTileAngelicCountingCloudExtra* AngelicCountingCloudExtra;
        WorldTileInfinityWeatherMachineExtra* InfinityWeatherMachineExtra;
        WorldTilePineappleGuzzlerExtra* PineappleGuzzlerExtra;
    } ExtraTileData{};
};