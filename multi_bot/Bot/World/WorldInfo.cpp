#include "WorldInfo.h"

#include <algorithm>

// faulty_tile = parsed tile that doesnt contain the extra tile data, but only the type.
// false = unsuccessful. probably unknown extra tile
// true = successful
bool WorldInfo::SerializeFromMem(std::vector<uint8_t> world_data_raw, std::optional<WorldTile>& faulty_tile) {
    Reset();

    Utils::BinaryStreamReader reader{ std::move(world_data_raw) };

    reader.Advance(6);
    m_world_name = reader.ReadString();
    m_width = reader.GenericRead<uint32_t>();
    m_height = reader.GenericRead<uint32_t>();
    
    m_tiles.reserve(reader.GenericRead<uint32_t>());

    reader.Advance(5);

    for (uint32_t i = 0; i < m_width * m_height; i++) {
        WorldTile temp{};
        bool successful = ParseTile(reader, temp, m_items_dat);

        if (!successful) {
            m_faulty_tile = temp;
            faulty_tile = temp;
            return false;
        }

        m_tiles.push_back(std::move(temp));
    }

    parse_item_drops(reader);

    m_base_weather = reader.GenericRead<uint16_t>();
    reader.Advance(2);
    m_current_weather = reader.GenericRead<uint16_t>();
    reader.Advance(6);

    m_parsed_succesfully = true;
    return true;
}

void WorldInfo::Reset() {
    m_width = 0;
    m_height = 0;
    m_last_obj_id = 0;
    m_base_weather = 0;
    m_current_weather = 0;
    m_world_name = "";
    m_dropped_items.clear();

    std::for_each(m_tiles.begin(), m_tiles.end(), [](WorldTile& tile) {
        tile.FreeTileExtra();
    });

    m_tiles.clear();
    m_parsed_succesfully = false;
    m_npc_list.clear();
    m_faulty_tile = {};
}

void WorldInfo::AddItemObject(uint32_t item_id, float pos_x, float pos_y, uint32_t amount) {
    WorldItemObject obj{};
    obj.ItemID = item_id;
    obj.PosX = pos_x;
    obj.PosY = pos_y;
    obj.ItemAmount = amount;
    obj.ObjectID = ++m_last_obj_id;

    m_dropped_items.insert_or_assign(obj.ObjectID, obj);
}

void WorldInfo::RemoveItemObject(WorldItemObjectID obj_id) {
    m_dropped_items.erase(obj_id);
}

WorldItemObject WorldInfo::GetItemObject(WorldItemObjectID obj_id) {
    auto it = m_dropped_items.find(obj_id);

    return it == m_dropped_items.end() ? WorldItemObject{} : it->second;
}

std::vector<WorldItemObject> WorldInfo::GetItemObjects() {
    std::vector<WorldItemObject> objs {};

    for (const auto& obj : m_dropped_items) {
        objs.push_back(obj.second);
    }

    return objs;
}

void WorldInfo::SetNPC(WorldNPC npc_data) {
    npc_data.LastUpdate = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
    m_npc_list.insert_or_assign(npc_data.Index, std::move(npc_data));
}

void WorldInfo::RemoveNPC(WorldNPCIndex npc_index) {
    m_npc_list.erase(npc_index);
}

std::optional<WorldNPC> WorldInfo::GetNPC(WorldNPCIndex npc_index) {
    auto found = m_npc_list.find(npc_index);

    return found != m_npc_list.end() ? found->second : std::optional<WorldNPC>{};
}

std::vector<WorldNPC> WorldInfo::GetNPCs() {
    std::vector<WorldNPC> objs{};

    for (const auto& obj : m_npc_list) {
        objs.push_back(obj.second);
    }

    return objs;
}

void WorldInfo::UpdateNPCsPos() {
    if (m_world_name.empty() || m_world_name == "EXIT") {
        return;
    }

    for (auto& npc : m_npc_list) {
        if (npc.second.PosX - npc.second.TargetX == 0 &&
            npc.second.PosY - npc.second.TargetY == 0
        ) {
            npc.second.LastUpdate = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
            continue;
        }

        float total_length = sqrtf(
            (fabsf(npc.second.PosX - npc.second.TargetX) * fabsf(npc.second.PosX - npc.second.TargetX)) +
            (fabsf(npc.second.PosY - npc.second.TargetY) * fabsf(npc.second.PosY - npc.second.TargetY))
        );

        float move_length = std::clamp((npc.second.Speed / 1000) * (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()) - npc.second.LastUpdate).count(), 0.0f, total_length);
        float ratio = move_length / total_length;

        npc.second.PosX += (npc.second.TargetX - npc.second.PosX) * ratio;
        npc.second.PosY += (npc.second.TargetY - npc.second.PosY) * ratio;
        npc.second.LastUpdate = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
    }
}


void WorldInfo::SetTile(uint32_t x, uint32_t y, WorldTile new_tile) {
    m_tiles.at((y * m_width) + x).FreeTileExtra();
    m_tiles.at((y * m_width) + x) = std::move(new_tile);
}

const WorldTile& WorldInfo::GetTileRef(uint32_t x, uint32_t y) {
    return m_tiles.at((y * m_width) + x);
}

// Pasted from https://github.com/badewen/GTProxy/blob/develop/src/world/world_info.cpp

// false = unsuccessful. probably unknown extra tile
// true = successful
bool WorldInfo::ParseTile(
    Utils::BinaryStreamReader& stream,
    WorldTile& tile,
    std::shared_ptr<ItemDatabase> items_dat
) {
    tile.Fg = stream.GenericRead<uint16_t>();
    tile.Bg = stream.GenericRead<uint16_t>();
    tile.ParentTileIndex = stream.GenericRead<uint16_t>();
    tile.Flags = stream.GenericRead<WorldTileFlag>();
    tile.ExtraTileDataType = eWorldTileExtraType::None;

    if (tile.Flags.bLocked) {
        tile.LockIndex = stream.GenericRead<uint16_t>();
    }

    if (tile.Flags.bTileExtra) {
        tile.ExtraTileDataType = stream.GenericRead<eWorldTileExtraType>();
        return ParseExtraTileData(tile, stream, items_dat);
    }

    return true;
}

// false = unsuccessful. probably unknown extra tile
// true = successful
bool WorldInfo::ParseExtraTileData(
    WorldTile& tile,
    Utils::BinaryStreamReader& stream, 
    std::shared_ptr<ItemDatabase> items_dat
) {
    tile.InitializeTileExtra(tile.ExtraTileDataType);

    switch (tile.ExtraTileDataType) {
    case None: {
        break;
    }
    case Door: {
        WorldTileDoorExtra* extra_data = tile.ExtraTileData.DoorExtra;
        extra_data->Label = stream.ReadString();
        extra_data->Unk1 = stream.GenericRead<uint8_t>();

        break;
    }

    case Sign: {
        WorldTileSignExtra* extra_data = tile.ExtraTileData.SignExtra;
        extra_data->Label = stream.ReadString();

        stream.Advance(4);

        break;
    }

    case Lock: {
        WorldTileLockExtra* extra_data = tile.ExtraTileData.LockExtra;
        extra_data->Flags = stream.GenericRead<uint8_t>();
        extra_data->OwnerId = stream.GenericRead<uint32_t>();
        extra_data->AccessList = stream.ReadList<uint32_t, uint32_t>();

        if (tile.Fg != 202 &&
            tile.Fg != 204 &&
            tile.Fg != 206 &&
            tile.Fg != 4994
            ) {
            extra_data->MinimumLevel = stream.GenericRead<uint8_t>();
            stream.ReadBytesToBuffer(extra_data->UnkArr, 7);
        }

        break;
    }

    case Seed: {
        WorldTileSeedExtra* extra_data = tile.ExtraTileData.SeedExtra;
        extra_data->TimeLeft = stream.GenericRead<uint32_t>();
        extra_data->FruitCount = stream.GenericRead<uint8_t>();

        break;
    }

    case Dice: {
        WorldTileDiceExtra* extra_data = tile.ExtraTileData.DiceExtra;
        extra_data->Pattern = stream.GenericRead<uint8_t>();

        break;
    }

    case Provider: {
        WorldTileProviderExtra* extra_data = tile.ExtraTileData.ProviderExtra;
        extra_data->TimeLeft = stream.GenericRead<uint32_t>();

        // well of love
        if (tile.Fg == 10656) {
            stream.Advance(4);
        }

        break;
    }

    case AchievementBlock: {
        WorldTileAchievementBlockExtra* extra_data = tile.ExtraTileData.AchievementBlockExtra;
        extra_data->UserId = stream.GenericRead<uint32_t>();
        extra_data->Achievement = stream.GenericRead<uint8_t>();

        break;
    }

    case HeartMonitor: {
        WorldTileHeartMonitorExtra* extra_data = tile.ExtraTileData.HeartMonitorExtra;
        extra_data->UserId = stream.GenericRead<uint32_t>();
        extra_data->Name = stream.ReadString();

        break;
    }

    case Mannequin: {
        WorldTileMannequinExtra* extra_data = tile.ExtraTileData.MannequinExtra;
        extra_data->Label = stream.ReadString();
        extra_data->Unk1 = stream.GenericRead<uint8_t>();
        extra_data->Unk2 = stream.GenericRead<uint16_t>();
        extra_data->Unk3 = stream.GenericRead<uint16_t>();
        extra_data->Hat = stream.GenericRead<uint16_t>();
        extra_data->Shirt = stream.GenericRead<uint16_t>();
        extra_data->Pants = stream.GenericRead<uint16_t>();
        extra_data->Boots = stream.GenericRead<uint16_t>();
        extra_data->Face = stream.GenericRead<uint16_t>();
        extra_data->Hand = stream.GenericRead<uint16_t>();
        extra_data->Back = stream.GenericRead<uint16_t>();
        extra_data->Hair = stream.GenericRead<uint16_t>();
        extra_data->Neck = stream.GenericRead<uint16_t>();

        break;
    }

    case MagicEgg: {
        WorldTileMagicEggExtra* extra_data = tile.ExtraTileData.MagicEggExtra;
        extra_data->EggAmount = stream.GenericRead<uint32_t>();

        break;
    }

    case GameGrave: {
        WorldTileGameGraveExtra* extra_data = tile.ExtraTileData.GameGraveExtra;
        extra_data->Team = stream.GenericRead<uint8_t>();

        break;
    }

    case GameGenerator: {
        WorldTileGameGeneratorExtra* extra_data = tile.ExtraTileData.GameGeneratorExtra;
        break;
    }

    case Xenonite: {
        WorldTileXenoniteExtra* extra_data = tile.ExtraTileData.XenoniteExtra;
        stream.ReadBytesToBuffer(extra_data->Unk1, 5);

        break;
    }

    case PhoneBooth: {
        WorldTilePhoneBoothExtra* extra_data = tile.ExtraTileData.PhoneBoothExtra;
        extra_data->Hat = stream.GenericRead<uint16_t>();
        extra_data->Shirt = stream.GenericRead<uint16_t>();
        extra_data->Pants = stream.GenericRead<uint16_t>();
        extra_data->Shoes = stream.GenericRead<uint16_t>();
        extra_data->Face = stream.GenericRead<uint16_t>();
        extra_data->Hand = stream.GenericRead<uint16_t>();
        extra_data->Back = stream.GenericRead<uint16_t>();
        extra_data->Hair = stream.GenericRead<uint16_t>();
        extra_data->Neck = stream.GenericRead<uint16_t>();

        break;
    }

    case Crystal: {
        WorldTileCrystalExtra* extra_data = tile.ExtraTileData.CrystalExtra;
        extra_data->Crystals = stream.ReadList<uint16_t, uint8_t>();

        break;
    }

    case CrimeInProgress: {
        WorldTileCrimeInProgressExtra* extra_data = tile.ExtraTileData.CrimeInProgressExtra;
        extra_data->Name = stream.ReadString();
        stream.Advance(5);

        break;
    }

    case Spotlight: {
        WorldTileSpotlightExtra* extra_data = tile.ExtraTileData.SpotlightExtra;
        break;
    }

    case DisplayBlock: {
        WorldTileDisplayBlockExtra* extra_data = tile.ExtraTileData.DisplayBlockExtra;
        extra_data->ItemId = stream.GenericRead<uint32_t>();

        break;
    }

    case VendingMachine: {
        WorldTileVendingMachineExtra* extra_data = tile.ExtraTileData.VendingMachineExtra;
        extra_data->ItemId = stream.GenericRead<uint32_t>();
        extra_data->Price = stream.GenericRead<int32_t>();

        break;
    }

    case FishTankPort: {
        WorldTileFishTankPortExtra* extra_data = tile.ExtraTileData.FishTankPortExtra;
        extra_data->Flags = stream.GenericRead<uint8_t>();
        uint32_t length = stream.GenericRead<uint32_t>();
        for (uint32_t i = 0; i < static_cast<uint32_t>(length / 2); i++) {
            WorldTileFishTankPortExtra::FishInfo fish_info{};

            fish_info.FishItemId = stream.GenericRead<uint32_t>();
            fish_info.Lbs = stream.GenericRead<uint32_t>();

            extra_data->Fishes.push_back(fish_info);
        }

        break;
    }

    case SolarCollector: {
        WorldTileSolarCollectorExtra* extra_data = tile.ExtraTileData.SolarCollectorExtra;
        stream.ReadBytesToBuffer(extra_data->Unk1, 5);

        break;
    }

    case Forge: {
        WorldTileForgeExtra* extra_data = tile.ExtraTileData.ForgeExtra;
        extra_data->Temperature = stream.GenericRead<uint32_t>();

        break;
    }

    case GivingTree: {
        WorldTileGivingTreeExtra* extra_data = tile.ExtraTileData.GivingTreeExtra;
        extra_data->Harvested = stream.GenericRead<uint8_t>();
        extra_data->Unk1 = stream.GenericRead<uint32_t>();
        extra_data->DecorationPercent = stream.GenericRead<uint8_t>();

        break;
    }

    case SteamOrgan: {
        WorldTileSteamOrganExtra* extra_data = tile.ExtraTileData.SteamOrganExtra;
        extra_data->InstrumentType = stream.GenericRead<uint8_t>();
        extra_data->Note = stream.GenericRead<uint32_t>();

        break;
    }

    case SilkWorm: {
        WorldTileSilkWormExtra* extra_data = tile.ExtraTileData.SilkWormExtra;
        extra_data->Type = stream.ReadByte();
        extra_data->Name = stream.ReadString();
        extra_data->Age = stream.GenericRead<uint32_t>();
        extra_data->Unk1 = stream.GenericRead<uint32_t>();
        extra_data->Unk2 = stream.GenericRead<uint32_t>();
        extra_data->CanBeFed = stream.ReadByte();
        extra_data->FoodSaturation = stream.GenericRead<uint32_t>();
        extra_data->WaterSaturation = stream.GenericRead<uint32_t>();
        stream.ReadBytesToBuffer((uint8_t*)&extra_data->Color, 4);
        extra_data->SickDuration = stream.GenericRead<uint32_t>();

        break;
    }

    case SewingMachine: {
        WorldTileSewingMachineExtra* extra_data = tile.ExtraTileData.SewingMachineExtra;
        extra_data->BoltIdList = stream.ReadList<uint32_t, uint32_t>();

        break;
    }

    case CountryFlag: {
        WorldTileCountryFlagExtra* extra_data = tile.ExtraTileData.CountryFlagExtra;

        if (items_dat->GetItemInfo(tile.Fg).ItemType != eItemInfoItemType::ChalFlag) {
            extra_data->Country = stream.ReadString();
        }

        break;
    }

    case LobsterTrap: {
        WorldTileLobsterTrapExtra* extra_data = tile.ExtraTileData.LobsterTrapExtra;
        break;
    }

    case PaintingEasel: {
        WorldTilePaintingEaselExtra* extra_data = tile.ExtraTileData.PaintingEaselExtra;
        extra_data->ItemId = stream.GenericRead<uint32_t>();
        extra_data->Label = stream.ReadString();

        break;
    }

    case PetBattleCage: {
        WorldTilePetBattleCageExtra* extra_data = tile.ExtraTileData.PetBattleCageExtra;
        extra_data->Label = stream.ReadString();
        extra_data->BasePet = stream.GenericRead<uint32_t>();
        extra_data->CombinedPet1 = stream.GenericRead<uint32_t>();
        extra_data->CombinedPet2 = stream.GenericRead<uint32_t>();

        break;
    }

    case PetTrainer: {
        WorldTilePetTrainerExtra* extra_data = tile.ExtraTileData.PetTrainerExtra;
        extra_data->Name = stream.ReadString();
        uint32_t pet_battle_count = stream.GenericRead<uint32_t>();
        stream.Advance(4);

        for (uint32_t i = 0; i < pet_battle_count; i++) {
            extra_data->PetList.push_back(stream.GenericRead<uint32_t>());
        }

        break;
    }

    case SteamEngine: {
        WorldTileSteamEngineExtra* extra_data = tile.ExtraTileData.SteamEngineExtra;
        extra_data->Temperature = stream.GenericRead<uint32_t>();

        break;
    }

    case LockBot: {
        WorldTileLockBotExtra* extra_data = tile.ExtraTileData.LockBotExtra;
        extra_data->TimePassedSec = stream.GenericRead<uint32_t>();

        break;
    }

    case WeatherMachine: {
        WorldTileWeatherMachineExtra* extra_data = tile.ExtraTileData.WeatherMachineExtra;
        extra_data->Flags = stream.GenericRead<uint32_t>();

        break;
    }

    case SpiritStorageUnit: {
        WorldTileSpiritStorageUnitExtra* extra_data = tile.ExtraTileData.SpiritStorageUnitExtra;
        extra_data->JarCount = stream.GenericRead<uint32_t>();

        break;
    }

    case DataBedrock: {
        WorldTileDataBedrockExtra* extra_data = tile.ExtraTileData.DataBedrockExtra;
        stream.ReadBytesToBuffer(extra_data->Unk1, 21);

        break;
    }

    case Shelf: {
        WorldTileShelfExtra* extra_data = tile.ExtraTileData.ShelfExtra;
        extra_data->TopLeft = stream.GenericRead<uint32_t>();
        extra_data->TopRight = stream.GenericRead<uint32_t>();
        extra_data->BottomLeft = stream.GenericRead<uint32_t>();
        extra_data->BottomRight = stream.GenericRead<uint32_t>();

        break;
    }

    case VipEntrance: {
        WorldTileVipEntranceExtra* extra_data = tile.ExtraTileData.VipEntranceExtra;
        extra_data->Unk1 = stream.GenericRead<uint8_t>();
        extra_data->OwnerId = stream.GenericRead<uint32_t>();
        extra_data->AllowedIdList = stream.ReadList<uint32_t, uint32_t>();

        break;
    }

    case ChallengeTimer: {
        WorldTileChallengeTimerExtra* extra_data = tile.ExtraTileData.ChallengeTimerExtra;
        break;
    }

    case FishWallMount: {
        WorldTileFishWallMountExtra* extra_data = tile.ExtraTileData.FishWallMountExtra;

        extra_data->Label = stream.ReadString();
        extra_data->ItemID = stream.GenericRead<uint32_t>();
        extra_data->FishLb = stream.GenericRead<uint8_t>();

        break;
    }

    case Portrait: {
        WorldTilePortraitExtra* extra_data = tile.ExtraTileData.PortraitExtra;
        extra_data->Label = stream.ReadString();
        extra_data->Unk1 = stream.GenericRead<uint32_t>();
        extra_data->Unk2 = stream.GenericRead<uint32_t>();
        extra_data->Unk3 = stream.GenericRead<uint32_t>();
        extra_data->Unk4 = stream.GenericRead<uint32_t>();
        extra_data->Face = stream.GenericRead<uint16_t>();
        extra_data->Hat = stream.GenericRead<uint16_t>();
        extra_data->Hair = stream.GenericRead<uint16_t>();
        extra_data->Unk5 = stream.GenericRead<uint16_t>();
        extra_data->Unk6 = stream.GenericRead<uint16_t>();

        break;
    }

    case GuildWeatherMachine: {
        WorldTileGuildWeatherMachineExtra* extra_data = tile.ExtraTileData.GuildWeatherMachineExtra;
        extra_data->Unk1 = stream.GenericRead<uint32_t>();
        extra_data->Gravity = stream.GenericRead<uint32_t>();
        extra_data->Flags = stream.GenericRead<uint8_t>();

        break;
    }

    case FossilPrepStation: {
        WorldTileFossilPrepStationExtra* extra_data = tile.ExtraTileData.FossilPrepStationExtra;
        extra_data->Unk1 = stream.GenericRead<uint32_t>();

        break;
    }

    case DnaExtractor: {
        WorldTileDnaExtractorExtra* extra_data = tile.ExtraTileData.DnaExtractorExtra;
        break;
    }

    case Howler: {
        WorldTileHowlerExtra* extra_data = tile.ExtraTileData.HowlerExtra;
        break;
    }

    case ChemsynthTank: {
        WorldTileChemsynthTankExtra* extra_data = tile.ExtraTileData.ChemsynthTankExtra;
        extra_data->CurrentChem = stream.GenericRead<uint32_t>();
        extra_data->TargetChem = stream.GenericRead<uint32_t>();

        break;
    }

    case StorageBlock: {
        WorldTileStorageBlockExtra* extra_data = tile.ExtraTileData.StorageBlockExtra;
        uint16_t length = stream.GenericRead<uint16_t>();

        for (uint16_t i = 0; i < static_cast<uint16_t>(length / 13); i++) {
            stream.Advance(3);
            uint32_t item_id = stream.GenericRead<uint32_t>();

            stream.Advance(2);
            uint32_t item_amount = stream.GenericRead<uint32_t>();

            WorldTileStorageBlockExtra::ItemInfo item_info{};
            item_info.ItemId = item_id;
            item_info.ItemAmount = item_amount;

            extra_data->Items.push_back(item_info);
        }

        break;
    }

    case CookingOven: {
        WorldTileCookingOvenExtra* extra_data = tile.ExtraTileData.CookingOvenExtra;

        extra_data->TemperatureLevel = stream.GenericRead<uint32_t>();
        uint32_t ingr_list_size = stream.GenericRead<uint32_t>();

        for (uint32_t i = 0; i < ingr_list_size; i++) {
            WorldTileCookingOvenExtra::IngredientInfo ingr_info{};

            ingr_info.ItemID = stream.GenericRead<uint32_t>();
            ingr_info.TimeAdded = stream.GenericRead<uint32_t>();
        }

        extra_data->Unk1 = stream.GenericRead<uint32_t>();
        extra_data->Unk1 = stream.GenericRead<uint32_t>();
        extra_data->Unk1 = stream.GenericRead<uint32_t>();

        break;
    }

    case AudioRack: {
        WorldTileAudioRackExtra* extra_data = tile.ExtraTileData.AudioRackExtra;
        extra_data->Note = stream.ReadString();
        extra_data->Volume = stream.GenericRead<uint32_t>();

        break;
    }

    case GeigerCharger: {
        WorldTileGeigerChargerExtra* extra_data = tile.ExtraTileData.GeigerChargerExtra;
        extra_data->Unk1 = stream.GenericRead<uint32_t>();

        break;
    }

    case AdventureBegins: {
        WorldTileAdventureBeginsExtra* extra_data = tile.ExtraTileData.AdventureBeginsExtra;
        break;
    }

    case TombRobber: {
        WorldTileTombRobberExtra* extra_data = tile.ExtraTileData.TombRobberExtra;
        break;
    }

    case BalloonOMatic: {
        WorldTileBalloonOMaticExtra* extra_data = tile.ExtraTileData.BalloonOMaticExtra;
        extra_data->TotalRarity = stream.GenericRead<uint32_t>();
        extra_data->TeamType = stream.GenericRead<uint8_t>();

        break;
    }

    case TrainingPort: {
        WorldTileTrainingPortExtra* extra_data = tile.ExtraTileData.TrainingPortExtra;
        extra_data->FishLb = stream.GenericRead<uint32_t>();
        extra_data->FishStatus = stream.GenericRead<uint16_t>();
        extra_data->FishId = stream.GenericRead<uint32_t>();
        extra_data->FishTotalExp = stream.GenericRead<uint32_t>();
        stream.ReadBytesToBuffer(extra_data->Unk1, 8);
        extra_data->FishLevel = stream.GenericRead<uint32_t>();
        extra_data->Unk2 = stream.GenericRead<uint32_t>();
        stream.ReadBytesToBuffer(extra_data->Unk3, 5);

        break;
    }

    case ItemSucker: {
        WorldTileItemSuckerExtra* extra_data = tile.ExtraTileData.ItemSuckerExtra;
        extra_data->ItemIdToSuck = stream.GenericRead<uint32_t>();
        extra_data->ItemAmount = stream.GenericRead<uint32_t>();
        extra_data->Flags = stream.GenericRead<uint16_t>();
        extra_data->Limit = stream.GenericRead<uint32_t>();

        break;
    }

    case CyBot: {
        WorldTileCyBotExtra* extra_data = tile.ExtraTileData.CyBotExtra;

        uint32_t command_count = stream.GenericRead<uint32_t>();

        for (uint32_t i = 0; i < command_count; i++) {
            WorldTileCyBotExtra::CommandData cmd_data{};
            cmd_data.CommandID = stream.GenericRead<uint32_t>();
            cmd_data.IsCommandUsed = stream.GenericRead<uint32_t>();
            stream.ReadBytesToBuffer(cmd_data.Unk1, 7);
            extra_data->CommandDatas.push_back(std::move(cmd_data));
        }

        extra_data->SyncTimer = stream.GenericRead<uint32_t>();
        extra_data->Activated = stream.GenericRead<uint32_t>();

        break;
    }

    case GuildItem: {
        WorldTileGuildItemExtra* extra_data = tile.ExtraTileData.GuildItemExtra;
        stream.ReadBytesToBuffer(extra_data->Unk1, 17);

        break;
    }

    case Growscan: {
        WorldTileGrowscanExtra* extra_data = tile.ExtraTileData.GrowscanExtra;
        extra_data->Unk1 = stream.GenericRead<uint8_t>();

        break;
    }

    case ContainmentFieldPowerNode: {
        WorldTileContainmentFieldPowerNodeExtra* extra_data = tile.ExtraTileData.ContainmentFieldPowerNodeExtra;
        extra_data->GhostJarCount = stream.GenericRead<uint32_t>();
        extra_data->Unk1 = std::move(stream.ReadList<uint32_t, uint32_t>());

        break;
    }

    case SpiritBoard: {
        WorldTileSpiritBoardExtra* extra_data = tile.ExtraTileData.SpiritBoardExtra;
        extra_data->Unk1 = stream.GenericRead<uint32_t>();
        extra_data->Unk2 = stream.GenericRead<uint32_t>();
        extra_data->Unk3 = stream.GenericRead<uint32_t>();

        break;
    }

    case StormyCloud: {
        WorldTileStormyCloudExtra* extra_data = tile.ExtraTileData.StormyCloudExtra;
        extra_data->StingDuration = stream.GenericRead<uint32_t>();
        extra_data->IsSolid = stream.GenericRead<uint32_t>();
        extra_data->NonSolidDuration = stream.GenericRead<uint32_t>();

        break;
    }

    case TemporaryPlatform: {
        WorldTileTemporaryPlatformExtra* extra_data = tile.ExtraTileData.TemporaryPlatformExtra;
        extra_data->Unk1 = stream.GenericRead<uint32_t>();

        break;
    }

    case SafeVault: {
        WorldTileSafeVaultExtra* extra_data = tile.ExtraTileData.SafeVaultExtra;
        break;
    }

    case AngelicCountingCloud: {
        WorldTileAngelicCountingCloudExtra* extra_data = tile.ExtraTileData.AngelicCountingCloudExtra;
        extra_data->IsRaffling = stream.GenericRead<uint32_t>();
        extra_data->Unk1 = stream.GenericRead<uint16_t>();
        extra_data->AsciiCode = stream.GenericRead<uint8_t>();

        break;
    }

    case InfinityWeatherMachine: {
        WorldTileInfinityWeatherMachineExtra* extra_data = tile.ExtraTileData.InfinityWeatherMachineExtra;
        extra_data->IntervalMinutes = stream.GenericRead<uint32_t>();
        extra_data->WeatherMachineList = stream.ReadList<uint32_t, uint32_t>();

        break;
    }

    case PineappleGuzzler: {
        WorldTilePineappleGuzzlerExtra* extra_data = tile.ExtraTileData.PineappleGuzzlerExtra;
        stream.Advance(4);

        break;
    }

    case KrakenGalaticBlock: {
        WorldTileKrakenGalaticBlockExtra* extra_data = tile.ExtraTileData.KrakenGalaticBlockExtra;
        extra_data->PatternIndex = stream.GenericRead<uint8_t>();
        extra_data->Unk1 = stream.GenericRead<uint32_t>();

        // this should be in the right order.
        extra_data->R = stream.GenericRead<uint8_t>();
        extra_data->G = stream.GenericRead<uint8_t>();
        extra_data->B = stream.GenericRead<uint8_t>();

        break;
    }

    case FriendsEntrance: {
        WorldTileFriendsEntranceExtra* extra_data = tile.ExtraTileData.FriendsEntranceExtra;
        extra_data->OwnerUserId = stream.GenericRead<uint32_t>();
        extra_data->Unk1 = stream.GenericRead<uint16_t>();
        extra_data->AllowedUserList = stream.ReadList<uint16_t, uint32_t>();

        break;
    }

    default: {
        // not handled. return false

        return false;
    }

    }
    return true;
}

void WorldInfo::parse_item_drops(Utils::BinaryStreamReader& stream) {
    // not a useful data for now.
    stream.Advance(12);

    uint32_t item_length = stream.GenericRead<uint32_t>();
    m_last_obj_id = stream.GenericRead<uint32_t>(); 

    for (uint32_t i = 0; i < item_length; i++) {
        WorldItemObject obj{};
        
        obj.ItemID = stream.GenericRead<uint16_t>();
        obj.PosX   = stream.ReadFloat();
        obj.PosY   = stream.ReadFloat();
        obj.ItemAmount = stream.GenericRead<uint8_t>();
        obj.Flags = stream.GenericRead<WorldItemObjectFlag>();
        obj.ObjectID = stream.GenericRead<uint32_t>();

        m_dropped_items.insert_or_assign(obj.ObjectID, obj);
    }
}
