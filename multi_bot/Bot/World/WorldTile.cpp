#include "WorldTile.h"

#include <assert.h>

void WorldTile::InitializeTileExtra(eWorldTileExtraType type) {
    ExtraTileDataType = type;

    switch (type)
    {
    case None:
        break;
    case Door:
        ExtraTileData.DoorExtra = new WorldTileDoorExtra();
        break;
    case Sign:
        ExtraTileData.SignExtra = new WorldTileSignExtra();
        break;
    case Lock:
        ExtraTileData.LockExtra = new WorldTileLockExtra();
        break;
    case Seed:
        ExtraTileData.SeedExtra = new WorldTileSeedExtra();
        break;
    case Dice:
        ExtraTileData.DiceExtra = new WorldTileDiceExtra();
        break;
    case Provider:
        ExtraTileData.ProviderExtra = new WorldTileProviderExtra();
        break;
    case AchievementBlock:
        ExtraTileData.AchievementBlockExtra = new WorldTileAchievementBlockExtra();
        break;
    case HeartMonitor:
        ExtraTileData.HeartMonitorExtra = new WorldTileHeartMonitorExtra();
        break;
    case Mannequin:
        ExtraTileData.MannequinExtra = new WorldTileMannequinExtra();
        break;
    case MagicEgg:
        ExtraTileData.MagicEggExtra = new WorldTileMagicEggExtra();
        break;
    case GameGrave:
        ExtraTileData.GameGraveExtra = new WorldTileGameGraveExtra();
        break;
    case GameGenerator:
        ExtraTileData.GameGeneratorExtra = new WorldTileGameGeneratorExtra();
        break;
    case Xenonite:
        ExtraTileData.XenoniteExtra = new WorldTileXenoniteExtra();
        break;
    case PhoneBooth:
        ExtraTileData.PhoneBoothExtra = new WorldTilePhoneBoothExtra();
        break;
    case Crystal:
        ExtraTileData.CrystalExtra = new WorldTileCrystalExtra();
        break;
    case CrimeInProgress:
        ExtraTileData.CrimeInProgressExtra = new WorldTileCrimeInProgressExtra();
        break;
    case Spotlight:
        ExtraTileData.SpotlightExtra = new WorldTileSpotlightExtra();
        break;
    case DisplayBlock:
        ExtraTileData.DisplayBlockExtra = new WorldTileDisplayBlockExtra();
        break;
    case VendingMachine:
        ExtraTileData.VendingMachineExtra = new WorldTileVendingMachineExtra();
        break;
    case FishTankPort:
        ExtraTileData.FishTankPortExtra = new WorldTileFishTankPortExtra();
        break;
    case SolarCollector:
        ExtraTileData.SolarCollectorExtra = new WorldTileSolarCollectorExtra();
        break;
    case Forge:
        ExtraTileData.ForgeExtra = new WorldTileForgeExtra();
        break;
    case GivingTree:
        ExtraTileData.GivingTreeExtra = new WorldTileGivingTreeExtra();
        break;
    case SteamOrgan:
        ExtraTileData.SteamOrganExtra = new WorldTileSteamOrganExtra();
        break;
    case SilkWorm:
        ExtraTileData.SilkWormExtra = new WorldTileSilkWormExtra();
        break;
    case SewingMachine:
        ExtraTileData.SewingMachineExtra = new WorldTileSewingMachineExtra();
        break;
    case CountryFlag:
        ExtraTileData.CountryFlagExtra = new WorldTileCountryFlagExtra();
        break;
    case LobsterTrap:
        ExtraTileData.LobsterTrapExtra = new WorldTileLobsterTrapExtra();
        break;
    case PaintingEasel:
        ExtraTileData.PaintingEaselExtra = new WorldTilePaintingEaselExtra();
        break;
    case PetBattleCage:
        ExtraTileData.PetBattleCageExtra = new WorldTilePetBattleCageExtra();
        break;
    case PetTrainer:
        ExtraTileData.PetTrainerExtra = new WorldTilePetTrainerExtra();
        break;
    case SteamEngine:
        ExtraTileData.SteamEngineExtra = new WorldTileSteamEngineExtra();
        break;
    case LockBot:
        ExtraTileData.LockBotExtra = new WorldTileLockBotExtra();
        break;
    case WeatherMachine:
        ExtraTileData.WeatherMachineExtra = new WorldTileWeatherMachineExtra();
        break;
    case SpiritStorageUnit:
        ExtraTileData.SpiritStorageUnitExtra = new WorldTileSpiritStorageUnitExtra();
        break;
    case DataBedrock:
        ExtraTileData.DataBedrockExtra = new WorldTileDataBedrockExtra();
        break;
    case Shelf:
        ExtraTileData.ShelfExtra = new WorldTileShelfExtra();
        break;
    case VipEntrance:
        ExtraTileData.VipEntranceExtra = new WorldTileVipEntranceExtra();
        break;
    case ChallengeTimer:
        ExtraTileData.ChallengeTimerExtra = new WorldTileChallengeTimerExtra();
        break;
    case FishWallMount:
        ExtraTileData.FishWallMountExtra = new WorldTileFishWallMountExtra();
        break;
    case Portrait:
        ExtraTileData.PortraitExtra = new WorldTilePortraitExtra();
        break;
    case GuildWeatherMachine:
        ExtraTileData.GuildWeatherMachineExtra = new WorldTileGuildWeatherMachineExtra();
        break;
    case FossilPrepStation:
        ExtraTileData.FossilPrepStationExtra = new WorldTileFossilPrepStationExtra();
        break;
    case DnaExtractor:
        ExtraTileData.DnaExtractorExtra = new WorldTileDnaExtractorExtra();
        break;
    case Howler:
        ExtraTileData.HowlerExtra = new WorldTileHowlerExtra();
        break;
    case ChemsynthTank:
        ExtraTileData.ChemsynthTankExtra = new WorldTileChemsynthTankExtra();
        break;
    case StorageBlock:
        ExtraTileData.StorageBlockExtra = new WorldTileStorageBlockExtra();
        break;
    case CookingOven:
        ExtraTileData.CookingOvenExtra = new WorldTileCookingOvenExtra();
        break;
    case AudioRack:
        ExtraTileData.AudioRackExtra = new WorldTileAudioRackExtra();
        break;
    case GeigerCharger:
        ExtraTileData.GeigerChargerExtra = new WorldTileGeigerChargerExtra();
        break;
    case AdventureBegins:
        ExtraTileData.AdventureBeginsExtra = new WorldTileAdventureBeginsExtra();
        break;
    case TombRobber:
        ExtraTileData.TombRobberExtra = new WorldTileTombRobberExtra();
        break;
    case BalloonOMatic:
        ExtraTileData.BalloonOMaticExtra = new WorldTileBalloonOMaticExtra();
        break;
    case TrainingPort:
        ExtraTileData.TrainingPortExtra = new WorldTileTrainingPortExtra();
        break;
    case ItemSucker:
        ExtraTileData.ItemSuckerExtra = new WorldTileItemSuckerExtra();
        break;
    case CyBot:
        ExtraTileData.CyBotExtra = new WorldTileCyBotExtra();
        break;
    case GuildItem:
        ExtraTileData.GuildItemExtra = new WorldTileGuildItemExtra();
        break;
    case Growscan:
        ExtraTileData.GrowscanExtra = new WorldTileGrowscanExtra();
        break;
    case ContainmentFieldPowerNode:
        ExtraTileData.ContainmentFieldPowerNodeExtra = new WorldTileContainmentFieldPowerNodeExtra();
        break;
    case SpiritBoard:
        ExtraTileData.SpiritBoardExtra = new WorldTileSpiritBoardExtra();
        break;
    case StormyCloud:
        ExtraTileData.StormyCloudExtra = new WorldTileStormyCloudExtra();
        break;
    case TemporaryPlatform:
        ExtraTileData.TemporaryPlatformExtra = new WorldTileTemporaryPlatformExtra();
        break;
    case SafeVault:
        ExtraTileData.SafeVaultExtra = new WorldTileSafeVaultExtra();
        break;
    case AngelicCountingCloud:
        ExtraTileData.AngelicCountingCloudExtra = new WorldTileAngelicCountingCloudExtra();
        break;
    case InfinityWeatherMachine:
        ExtraTileData.InfinityWeatherMachineExtra = new WorldTileInfinityWeatherMachineExtra();
        break;
    case PineappleGuzzler:
        ExtraTileData.PineappleGuzzlerExtra = new WorldTilePineappleGuzzlerExtra();
        break;
    case KrakenGalaticBlock:
        ExtraTileData.KrakenGalaticBlockExtra = new WorldTileKrakenGalaticBlockExtra();
        break;
    case FriendsEntrance:
        ExtraTileData.FriendsEntranceExtra = new WorldTileFriendsEntranceExtra();
        break;
    default:
        break;
    }
}

void WorldTile::FreeTileExtra() {
    switch (ExtraTileDataType)
    {
    case None:
        break;
    case Door:
        delete ExtraTileData.DoorExtra;
        break;
    case Sign:
        delete ExtraTileData.SignExtra;
        break;
    case Lock:
        delete ExtraTileData.LockExtra;
        break;
    case Seed:
        delete ExtraTileData.SeedExtra;
        break;
    case Dice:
        delete ExtraTileData.DiceExtra;
        break;
    case Provider:
        delete ExtraTileData.ProviderExtra;
        break;
    case AchievementBlock:
        delete ExtraTileData.AchievementBlockExtra;
        break;
    case HeartMonitor:
        delete ExtraTileData.HeartMonitorExtra;
        break;
    case Mannequin:
        delete ExtraTileData.MannequinExtra;
        break;
    case MagicEgg:
        delete ExtraTileData.MagicEggExtra;
        break;
    case GameGrave:
        delete ExtraTileData.GameGraveExtra;
        break;
    case GameGenerator:
        delete ExtraTileData.GameGeneratorExtra;
        break;
    case Xenonite:
        delete ExtraTileData.XenoniteExtra;
        break;
    case PhoneBooth:
        delete ExtraTileData.PhoneBoothExtra;
        break;
    case Crystal:
        delete ExtraTileData.CrystalExtra;
        break;
    case CrimeInProgress:
        delete ExtraTileData.CrimeInProgressExtra;
        break;
    case Spotlight:
        delete ExtraTileData.SpotlightExtra;
        break;
    case DisplayBlock:
        delete ExtraTileData.DisplayBlockExtra;
        break;
    case VendingMachine:
        delete ExtraTileData.VendingMachineExtra;
        break;
    case FishTankPort:
        delete ExtraTileData.FishTankPortExtra;
        break;
    case SolarCollector:
        delete ExtraTileData.SolarCollectorExtra;
        break;
    case Forge:
        delete ExtraTileData.ForgeExtra;
        break;
    case GivingTree:
        delete ExtraTileData.GivingTreeExtra;
        break;
    case SteamOrgan:
        delete ExtraTileData.SteamOrganExtra;
        break;
    case SilkWorm:
        delete ExtraTileData.SilkWormExtra;
        break;
    case SewingMachine:
        delete ExtraTileData.SewingMachineExtra;
        break;
    case CountryFlag:
        delete ExtraTileData.CountryFlagExtra;
        break;
    case LobsterTrap:
        delete ExtraTileData.LobsterTrapExtra;
        break;
    case PaintingEasel:
        delete ExtraTileData.PaintingEaselExtra;
        break;
    case PetBattleCage:
        delete ExtraTileData.PetBattleCageExtra;
        break;
    case PetTrainer:
        delete ExtraTileData.PetTrainerExtra;
        break;
    case SteamEngine:
        delete ExtraTileData.SteamEngineExtra;
        break;
    case LockBot:
        delete ExtraTileData.LockBotExtra;
        break;
    case WeatherMachine:
        delete ExtraTileData.WeatherMachineExtra;
        break;
    case SpiritStorageUnit:
        delete ExtraTileData.SpiritStorageUnitExtra;
        break;
    case DataBedrock:
        delete ExtraTileData.DataBedrockExtra;
        break;
    case Shelf:
        delete ExtraTileData.ShelfExtra;
        break;
    case VipEntrance:
        delete ExtraTileData.VipEntranceExtra;
        break;
    case ChallengeTimer:
        delete ExtraTileData.ChallengeTimerExtra;
        break;
    case FishWallMount:
        delete ExtraTileData.FishWallMountExtra;
        break;
    case Portrait:
        delete ExtraTileData.PortraitExtra;
        break;
    case GuildWeatherMachine:
        delete ExtraTileData.GuildWeatherMachineExtra;
        break;
    case FossilPrepStation:
        delete ExtraTileData.FossilPrepStationExtra;
        break;
    case DnaExtractor:
        delete ExtraTileData.DnaExtractorExtra;
        break;
    case Howler:
        delete ExtraTileData.HowlerExtra;
        break;
    case ChemsynthTank:
        delete ExtraTileData.ChemsynthTankExtra;
        break;
    case StorageBlock:
        delete ExtraTileData.StorageBlockExtra;
        break;
    case CookingOven:
        delete ExtraTileData.CookingOvenExtra;
        break;
    case AudioRack:
        delete ExtraTileData.AudioRackExtra;
        break;
    case GeigerCharger:
        delete ExtraTileData.GeigerChargerExtra;
        break;
    case AdventureBegins:
        delete ExtraTileData.AdventureBeginsExtra;
        break;
    case TombRobber:
        delete ExtraTileData.TombRobberExtra;
        break;
    case BalloonOMatic:
        delete ExtraTileData.BalloonOMaticExtra;
        break;
    case TrainingPort:
        delete ExtraTileData.TrainingPortExtra;
        break;
    case ItemSucker:
        delete ExtraTileData.ItemSuckerExtra;
        break;
    case CyBot:
        delete ExtraTileData.CyBotExtra;
        break;
    case GuildItem:
        delete ExtraTileData.GuildItemExtra;
        break;
    case Growscan:
        delete ExtraTileData.GrowscanExtra;
        break;
    case ContainmentFieldPowerNode:
        delete ExtraTileData.ContainmentFieldPowerNodeExtra;
        break;
    case SpiritBoard:
        delete ExtraTileData.SpiritBoardExtra;
        break;
    case StormyCloud:
        delete ExtraTileData.StormyCloudExtra;
        break;
    case TemporaryPlatform:
        delete ExtraTileData.TemporaryPlatformExtra;
        break;
    case SafeVault:
        delete ExtraTileData.SafeVaultExtra;
        break;
    case AngelicCountingCloud:
        delete ExtraTileData.AngelicCountingCloudExtra;
        break;
    case InfinityWeatherMachine:
        delete ExtraTileData.InfinityWeatherMachineExtra;
        break;
    case PineappleGuzzler:
        delete ExtraTileData.PineappleGuzzlerExtra;
        break;
    case KrakenGalaticBlock:
        delete ExtraTileData.KrakenGalaticBlockExtra;
        break;
    case FriendsEntrance:
        delete ExtraTileData.FriendsEntranceExtra;
        break;
    default:
        break;
    }

    ExtraTileData._struct_buff_ptr = nullptr;
    ExtraTileDataType = eWorldTileExtraType::None;
}
