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
    switch (tile.ExtraTileDataType) {
    case None: {
        break;
    }
    case Door: {
        WorldTileDoorExtra extra_data {};
        extra_data.Label = stream.ReadString();
        extra_data.Unk1 = stream.GenericRead<uint8_t>();

        *tile.ExtraTileData.DoorExtra = std::move(extra_data);
        break;
    }

    case Sign: {
        WorldTileSignExtra extra_data {};
        extra_data.Label = stream.ReadString();

        stream.Advance(4);

        *tile.ExtraTileData.SignExtra = std::move(extra_data);
        break;
    }

    case Lock: {
        WorldTileLockExtra extra_data {};
        extra_data.Flags = stream.GenericRead<uint8_t>();
        extra_data.OwnerId = stream.GenericRead<uint32_t>();
        extra_data.AccessList = stream.ReadList<uint32_t, uint32_t>();

        if (tile.Fg != 202 &&
            tile.Fg != 204 &&
            tile.Fg != 206 &&
            tile.Fg != 4994
        ) {
            extra_data.MinimumLevel = stream.GenericRead<uint8_t>();
            stream.ReadBytesToBuffer(extra_data.UnkArr, 7);
        }

        *tile.ExtraTileData.LockExtra = std::move(extra_data);
        break;
    }

    case Seed: {
        WorldTileSeedExtra extra_data {};
        extra_data.TimeLeft = stream.GenericRead<uint32_t>();
        extra_data.FruitCount = stream.GenericRead<uint8_t>();

        *tile.ExtraTileData.SeedExtra = std::move(extra_data);
        break;
    }

    case Dice: {
        WorldTileDiceExtra extra_data {};
        extra_data.Pattern = stream.GenericRead<uint8_t>();

        *tile.ExtraTileData.DiceExtra = std::move(extra_data);
        break;
    }

    case Provider: {
        WorldTileProviderExtra extra_data {};
        extra_data.TimeLeft = stream.GenericRead<uint32_t>();

        // well of love
        if (tile.Fg == 10656) {
            stream.Advance(4);
        }

        *tile.ExtraTileData.ProviderExtra = std::move(extra_data);
        break;
    }

    case AchievementBlock: {
        WorldTileAchievementBlockExtra extra_data {};
        extra_data.UserId = stream.GenericRead<uint32_t>();
        extra_data.Achievement = stream.GenericRead<uint8_t>();

        *tile.ExtraTileData.AchievementBlockExtra = std::move(extra_data);
        break;
    }

    case HeartMonitor: {
        WorldTileHeartMonitorExtra extra_data {};
        extra_data.UserId = stream.GenericRead<uint32_t>();
        extra_data.Name = stream.ReadString();

        *tile.ExtraTileData.HeartMonitorExtra = std::move(extra_data);
        break;
    }

    case Mannequin: {
        WorldTileMannequinExtra extra_data {};
        extra_data.Label = stream.ReadString();
        extra_data.Unk1 = stream.GenericRead<uint8_t>();
        extra_data.Unk2 = stream.GenericRead<uint16_t>();
        extra_data.Unk3 = stream.GenericRead<uint16_t>();
        extra_data.Hat = stream.GenericRead<uint16_t>();
        extra_data.Shirt = stream.GenericRead<uint16_t>();
        extra_data.Pants = stream.GenericRead<uint16_t>();
        extra_data.Boots = stream.GenericRead<uint16_t>();
        extra_data.Face = stream.GenericRead<uint16_t>();
        extra_data.Hand = stream.GenericRead<uint16_t>();
        extra_data.Back = stream.GenericRead<uint16_t>();
        extra_data.Hair = stream.GenericRead<uint16_t>();
        extra_data.Neck = stream.GenericRead<uint16_t>();

        *tile.ExtraTileData.MannequinExtra = std::move(extra_data);
        break;
    }

    case MagicEgg: {
        WorldTileMagicEggExtra extra_data{};
        extra_data.EggAmount = stream.GenericRead<uint32_t>();

        *tile.ExtraTileData.MagicEggExtra = std::move(extra_data);
        break;
    }

    case GameGrave: {
        WorldTileGameGraveExtra extra_data {};
        extra_data.Team = stream.GenericRead<uint8_t>();

        *tile.ExtraTileData.GameGraveExtra = std::move(extra_data);
        break;
    }

    case GameGenerator: {
        WorldTileGameGeneratorExtra extra_data {};
        *tile.ExtraTileData.GameGeneratorExtra = std::move(extra_data);
        break;
    }

    case Xenonite: {
        WorldTileXenoniteExtra extra_data {};
        stream.ReadBytesToBuffer(extra_data.Unk1, 5);

        *tile.ExtraTileData.XenoniteExtra = std::move(extra_data);
        break;
    }

    case PhoneBooth: {
        WorldTilePhoneBoothExtra extra_data {};
        extra_data.Hat = stream.GenericRead<uint16_t>();
        extra_data.Shirt = stream.GenericRead<uint16_t>();
        extra_data.Pants = stream.GenericRead<uint16_t>();
        extra_data.Shoes = stream.GenericRead<uint16_t>();
        extra_data.Face = stream.GenericRead<uint16_t>();
        extra_data.Hand = stream.GenericRead<uint16_t>();
        extra_data.Back = stream.GenericRead<uint16_t>();
        extra_data.Hair = stream.GenericRead<uint16_t>();
        extra_data.Neck = stream.GenericRead<uint16_t>();

        *tile.ExtraTileData.PhoneBoothExtra = std::move(extra_data);
        break;
    }

    case Crystal: {
        WorldTileCrystalExtra extra_data{};
        extra_data.Crystals = stream.ReadList<uint16_t, uint8_t>();

        *tile.ExtraTileData.CrystalExtra = std::move(extra_data);
        break;
    }

    case CrimeInProgress: {
        WorldTileCrimeInProgressExtra extra_data{};
        extra_data.Name = stream.ReadString();
        stream.Advance(5);

        *tile.ExtraTileData.CrimeInProgressExtra = std::move(extra_data);
        break;
    }

    case Spotlight: {
        WorldTileSpotlightExtra extra_data {};
        *tile.ExtraTileData.SpotlightExtra = std::move(extra_data);
        break;
    }

    case DisplayBlock: {
        WorldTileDisplayBlockExtra extra_data {};
        extra_data.ItemId = stream.GenericRead<uint32_t>();

        *tile.ExtraTileData.DisplayBlockExtra = std::move(extra_data);
        break;
    }

    case VendingMachine: {
        WorldTileVendingMachineExtra extra_data {};
        extra_data.ItemId = stream.GenericRead<uint32_t>();
        extra_data.Price = stream.GenericRead<int32_t>();

        *tile.ExtraTileData.VendingMachineExtra = std::move(extra_data);
        break;
    }

    case FishTankPort: {
        WorldTileFishTankPortExtra extra_data {};
        extra_data.Flags = stream.GenericRead<uint8_t>();
        uint32_t length = stream.GenericRead<uint32_t>();
        for (uint32_t i = 0; i < static_cast<uint32_t>(length / 2); i++) {
            WorldTileFishTankPortExtra::FishInfo fish_info{};

            fish_info.FishItemId = stream.GenericRead<uint32_t>();
            fish_info.Lbs = stream.GenericRead<uint32_t>();

            extra_data.Fishes.push_back(fish_info);
        }

        *tile.ExtraTileData.FishTankPortExtra = std::move(extra_data);
        break;
    }

    case SolarCollector: {
        WorldTileSolarCollectorExtra extra_data{};
        stream.ReadBytesToBuffer(extra_data.Unk1, 5);

        *tile.ExtraTileData.SolarCollectorExtra = std::move(extra_data);
        break;
    }

    case Forge: {
        WorldTileForgeExtra extra_data {};
        extra_data.Temperature = stream.GenericRead<uint32_t>();

        *tile.ExtraTileData.ForgeExtra = std::move(extra_data);
        break;
    }

    case GivingTree: {
        WorldTileGivingTreeExtra extra_data {};
        extra_data.Harvested = stream.GenericRead<uint8_t>();
        extra_data.Unk1 = stream.GenericRead<uint32_t>();
        extra_data.DecorationPercent = stream.GenericRead<uint8_t>();

        *tile.ExtraTileData.GivingTreeExtra = std::move(extra_data);
        break;
    }

    case SteamOrgan: {
        WorldTileSteamOrganExtra extra_data{};
        extra_data.InstrumentType = stream.GenericRead<uint8_t>();
        extra_data.Note = stream.GenericRead<uint32_t>();

        *tile.ExtraTileData.SteamOrganExtra = std::move(extra_data);
        break;
    }

    case SilkWorm: {
        WorldTileSilkWormExtra extra_data{};
        extra_data.Type = stream.ReadByte();
        extra_data.Name = stream.ReadString();
        extra_data.Age = stream.GenericRead<uint32_t>();
        extra_data.Unk1 = stream.GenericRead<uint32_t>();
        extra_data.Unk2 = stream.GenericRead<uint32_t>();
        extra_data.CanBeFed = stream.ReadByte();
        extra_data.FoodSaturation = stream.GenericRead<uint32_t>();
        extra_data.WaterSaturation = stream.GenericRead<uint32_t>();
        stream.ReadBytesToBuffer((uint8_t*) & extra_data.Color, 4);
        extra_data.SickDuration = stream.GenericRead<uint32_t>();

        *tile.ExtraTileData.SilkWormExtra = std::move(extra_data);
        break;
    }

    case SewingMachine: {
        WorldTileSewingMachineExtra extra_data {};        
        extra_data.BoltIdList = stream.ReadList<uint32_t, uint32_t>();
        
         *tile.ExtraTileData.SewingMachineExtra = std::move(extra_data);
         break;
    }

    case CountryFlag: {
        WorldTileCountryFlagExtra extra_data {};

        if (items_dat->GetItemInfo(tile.Fg).ItemType != eItemInfoItemType::ChalFlag) {
            extra_data.Country = stream.ReadString();
        }

        *tile.ExtraTileData.CountryFlagExtra = std::move(extra_data);
        break;
    }

    case LobsterTrap: {
        WorldTileLobsterTrapExtra extra_data {};
        *tile.ExtraTileData.LobsterTrapExtra = std::move(extra_data);
        break;
    }

    case PaintingEasel: {
        WorldTilePaintingEaselExtra extra_data {};
        extra_data.ItemId = stream.GenericRead<uint32_t>();
        extra_data.Label = stream.ReadString();

        *tile.ExtraTileData.PaintingEaselExtra = std::move(extra_data);
        break;
    }

    case PetBattleCage: {
        WorldTilePetBattleCageExtra extra_data{};
        extra_data.Label = stream.ReadString();
        extra_data.BasePet = stream.GenericRead<uint32_t>();
        extra_data.CombinedPet1 = stream.GenericRead<uint32_t>();
        extra_data.CombinedPet2 = stream.GenericRead<uint32_t>();

        *tile.ExtraTileData.PetBattleCageExtra = std::move(extra_data);
        break;
    }

    case PetTrainer: {
        WorldTilePetTrainerExtra extra_data{};
        extra_data.Name = stream.ReadString();
        uint32_t pet_battle_count = stream.GenericRead<uint32_t>();
        stream.Advance(4);

        for (uint32_t i = 0; i < pet_battle_count; i++) {
            extra_data.PetList.push_back(stream.GenericRead<uint32_t>());
        }

        *tile.ExtraTileData.PetTrainerExtra = std::move(extra_data);
        break;
    }

    case SteamEngine: {
        WorldTileSteamEngineExtra extra_data{};
        extra_data.Temperature = stream.GenericRead<uint32_t>();

        *tile.ExtraTileData.SteamEngineExtra = std::move(extra_data);
        break;
    }

    case LockBot: {
        WorldTileLockBotExtra extra_data{};
        extra_data.TimePassedSec = stream.GenericRead<uint32_t>();

        *tile.ExtraTileData.LockBotExtra = std::move(extra_data);
        break;
    }

    case WeatherMachine: {
        WorldTileWeatherMachineExtra extra_data {};
        extra_data.Flags = stream.GenericRead<uint32_t>();

        *tile.ExtraTileData.WeatherMachineExtra = std::move(extra_data);
        break;
    }

    case SpiritStorageUnit: {
        WorldTileSpiritStorageUnitExtra extra_data{};
        extra_data.JarCount = stream.GenericRead<uint32_t>();

        *tile.ExtraTileData.SpiritStorageUnitExtra = std::move(extra_data);
        break;
    }

    case DataBedrock: {
        WorldTileDataBedrockExtra extra_data {};
        stream.ReadBytesToBuffer(extra_data.Unk1, 21);

        *tile.ExtraTileData.DataBedrockExtra = std::move(extra_data);
        break;
    }

    case Shelf: {
        WorldTileShelfExtra extra_data {};
        extra_data.TopLeft = stream.GenericRead<uint32_t>();
        extra_data.TopRight = stream.GenericRead<uint32_t>();
        extra_data.BottomLeft = stream.GenericRead<uint32_t>();
        extra_data.BottomRight = stream.GenericRead<uint32_t>();

        *tile.ExtraTileData.ShelfExtra = std::move(extra_data);
        break;
    }

    case VipEntrance: {
        WorldTileVipEntranceExtra extra_data {};
        extra_data.Unk1 = stream.GenericRead<uint8_t>();
        extra_data.OwnerId = stream.GenericRead<uint32_t>();
        extra_data.AllowedIdList = stream.ReadList<uint32_t, uint32_t>();

        *tile.ExtraTileData.VipEntranceExtra = std::move(extra_data);
        break;
    }

    case ChallengeTimer: {
         WorldTileChallengeTimerExtra extra_data {};
        *tile.ExtraTileData.ChallengeTimerExtra = std::move(extra_data);
        break;
    }

    case FishWallMount: {
        WorldTileFishWallMountExtra extra_data{};

        extra_data.Label = stream.ReadString();
        extra_data.ItemID = stream.GenericRead<uint32_t>();
        extra_data.FishLb = stream.GenericRead<uint8_t>();

        *tile.ExtraTileData.FishWallMountExtra = std::move(extra_data);
        break;
    }

    case Portrait: {
        WorldTilePortraitExtra extra_data {};
        extra_data.Label = stream.ReadString();
        extra_data.Unk1 = stream.GenericRead<uint32_t>();
        extra_data.Unk2 = stream.GenericRead<uint32_t>();
        extra_data.Unk3 = stream.GenericRead<uint32_t>();
        extra_data.Unk4 = stream.GenericRead<uint32_t>();
        extra_data.Face = stream.GenericRead<uint16_t>();
        extra_data.Hat = stream.GenericRead<uint16_t>();
        extra_data.Hair = stream.GenericRead<uint16_t>();
        extra_data.Unk5 = stream.GenericRead<uint16_t>();
        extra_data.Unk6 = stream.GenericRead<uint16_t>();

        *tile.ExtraTileData.PortraitExtra = std::move(extra_data);
        break;
    }

    case GuildWeatherMachine: {
        WorldTileGuildWeatherMachineExtra extra_data {};
        extra_data.Unk1 = stream.GenericRead<uint32_t>();
        extra_data.Gravity = stream.GenericRead<uint32_t>();
        extra_data.Flags = stream.GenericRead<uint8_t>();

        *tile.ExtraTileData.GuildWeatherMachineExtra = std::move(extra_data);
        break;
    }

    case FossilPrepStation: {
        WorldTileFossilPrepStationExtra extra_data{};
        extra_data.Unk1 = stream.GenericRead<uint32_t>();

        *tile.ExtraTileData.FossilPrepStationExtra = std::move(extra_data);
        break;
    }

    case DnaExtractor: {
        WorldTileDnaExtractorExtra extra_data {};
        *tile.ExtraTileData.DnaExtractorExtra = std::move(extra_data);
        break;
    }

    case Howler: {
        WorldTileHowlerExtra extra_data {};
        *tile.ExtraTileData.HowlerExtra = std::move(extra_data);
        break;
    }

    case ChemsynthTank: {
        WorldTileChemsynthTankExtra extra_data{};
        extra_data.CurrentChem = stream.GenericRead<uint32_t>();
        extra_data.TargetChem = stream.GenericRead<uint32_t>();

        *tile.ExtraTileData.ChemsynthTankExtra = std::move(extra_data);
        break;
    }

    case StorageBlock: {
        WorldTileStorageBlockExtra extra_data {};
        uint16_t length = stream.GenericRead<uint16_t>();

        for (uint16_t i = 0; i < static_cast<uint16_t>(length / 13); i++) {
            stream.Advance(3);
            uint32_t item_id = stream.GenericRead<uint32_t>();

            stream.Advance(2);
            uint32_t item_amount = stream.GenericRead<uint32_t>();

            WorldTileStorageBlockExtra::ItemInfo item_info{};
            item_info.ItemId = item_id;
            item_info.ItemAmount = item_amount;

            extra_data.Items.push_back(item_info);
        }

        *tile.ExtraTileData.StorageBlockExtra = std::move(extra_data);
        break;
    }

    case CookingOven: {
        WorldTileCookingOvenExtra extra_data {};

        extra_data.TemperatureLevel = stream.GenericRead<uint32_t>();
        uint32_t ingr_list_size = stream.GenericRead<uint32_t>();

        for (uint32_t i = 0; i < ingr_list_size; i++) {
            WorldTileCookingOvenExtra::IngredientInfo ingr_info{};

            ingr_info.ItemID = stream.GenericRead<uint32_t>();
            ingr_info.TimeAdded = stream.GenericRead<uint32_t>();
        }

        extra_data.Unk1 = stream.GenericRead<uint32_t>();
        extra_data.Unk1 = stream.GenericRead<uint32_t>();
        extra_data.Unk1 = stream.GenericRead<uint32_t>();

        *tile.ExtraTileData.CookingOvenExtra = std::move(extra_data);
        break;
    }

    case AudioRack: {
        WorldTileAudioRackExtra extra_data {};
        extra_data.Note = stream.ReadString();
        extra_data.Volume = stream.GenericRead<uint32_t>();

        *tile.ExtraTileData.AudioRackExtra = std::move(extra_data);
        break;
    }

    case GeigerCharger: {
        WorldTileGeigerChargerExtra extra_data{};
        extra_data.Unk1 = stream.GenericRead<uint32_t>();

        *tile.ExtraTileData.GeigerChargerExtra = std::move(extra_data);
        break;
    }

    case AdventureBegin: {
        WorldTileAdventureBeginsExtra extra_data {};
        *tile.ExtraTileData.AdventureBeginsExtra = std::move(extra_data);
        break;
    }

    case TombRobber: {
        WorldTileTombRobberExtra extra_data {};
        *tile.ExtraTileData.TombRobberExtra = std::move(extra_data);
        break;
    }

    case BalloonOMatic: {
        WorldTileBalloonOMaticExtra extra_data{};
        extra_data.TotalRarity = stream.GenericRead<uint32_t>();
        extra_data.TeamType = stream.GenericRead<uint8_t>();

        *tile.ExtraTileData.BalloonOMaticExtra = std::move(extra_data);
        break;  
    }

    case TrainingPort: {
        WorldTileTrainingPortExtra extra_data {};
        extra_data.FishLb = stream.GenericRead<uint32_t>();
        extra_data.FishStatus = stream.GenericRead<uint16_t>();
        extra_data.FishId = stream.GenericRead<uint32_t>();
        extra_data.FishTotalExp = stream.GenericRead<uint32_t>();
        stream.ReadBytesToBuffer(extra_data.Unk1, 8);
        extra_data.FishLevel = stream.GenericRead<uint32_t>();
        extra_data.Unk2 = stream.GenericRead<uint32_t>();
        stream.ReadBytesToBuffer(extra_data.Unk3, 5);

        *tile.ExtraTileData.TrainingPortExtra = std::move(extra_data);
        break;
    }

    case ItemSucker: {
        WorldTileItemSuckerExtra extra_data{};
        extra_data.ItemIdToSuck = stream.GenericRead<uint32_t>();
        extra_data.ItemAmount = stream.GenericRead<uint32_t>();
        extra_data.Flags = stream.GenericRead<uint16_t>();
        extra_data.Limit = stream.GenericRead<uint32_t>();

        *tile.ExtraTileData.ItemSuckerExtra = std::move(extra_data);
        break;
    }

    case CyBot: {
        WorldTileCyBotExtra extra_data{};

        uint32_t command_count = stream.GenericRead<uint32_t>();

        for (uint32_t i = 0; i < command_count; i++) {
            WorldTileCyBotExtra::CommandData cmd_data{};
            cmd_data.CommandID = stream.GenericRead<uint32_t>();
            cmd_data.IsCommandUsed = stream.GenericRead<uint32_t>();
            stream.ReadBytesToBuffer(cmd_data.Unk1, 7);
            extra_data.CommandDatas.push_back(std::move(cmd_data));
        }

        extra_data.SyncTimer = stream.GenericRead<uint32_t>();
        extra_data.Activated = stream.GenericRead<uint32_t>();

        *tile.ExtraTileData.CyBotExtra = std::move(extra_data);
        break; 
    }

    case GuildItem: {
        WorldTileGuildItemExtra extra_data {};
        stream.ReadBytesToBuffer(extra_data.Unk1, 17);

        *tile.ExtraTileData.GuildItemExtra = std::move(extra_data);
        break;
    }

    case Growscan: {
        WorldTileGrowscanExtra extra_data{};
        extra_data.Unk1 = stream.GenericRead<uint8_t>();

        *tile.ExtraTileData.GrowscanExtra = std::move(extra_data);
        break;
    }

    case ContainmentFieldPowerNode: {
        WorldTileContainmentFieldPowerNodeExtra extra_data{};
        extra_data.GhostJarCount = stream.GenericRead<uint32_t>();
        extra_data.Unk1 = std::move(stream.ReadList<uint32_t, uint32_t>());

        *tile.ExtraTileData.ContainmentFieldPowerNodeExtra = std::move(extra_data);
        break;
    }

    case SpiritBoard: {
        WorldTileSpiritBoardExtra extra_data{};
        extra_data.Unk1 = stream.GenericRead<uint32_t>();
        extra_data.Unk2 = stream.GenericRead<uint32_t>();
        extra_data.Unk3 = stream.GenericRead<uint32_t>();

        *tile.ExtraTileData.SpiritBoardExtra = std::move(extra_data);
        break;  
    }

    case StormyCloud: {
        WorldTileStormyCloudExtra extra_data{};
        extra_data.StingDuration = stream.GenericRead<uint32_t>();
        extra_data.IsSolid = stream.GenericRead<uint32_t>();
        extra_data.NonSolidDuration = stream.GenericRead<uint32_t>();

        *tile.ExtraTileData.StormyCloudExtra = std::move(extra_data);
        break;
    }

    case TemporaryPlatform: {
        WorldTileTemporaryPlatformExtra extra_data{};
        extra_data.Unk1 = stream.GenericRead<uint32_t>();

        *tile.ExtraTileData.TemporaryPlatformExtra = std::move(extra_data);
        break;
    }
 
    case SafeVault: {
        WorldTileSafeVaultExtra extra_data{};
        *tile.ExtraTileData.SafeVaultExtra = std::move(extra_data);
        break;
    }

    case AngelicCountingCloud: {
        WorldTileAngelicCountingCloudExtra extra_data{};
        extra_data.IsRaffling = stream.GenericRead<uint32_t>();
        extra_data.Unk1 = stream.GenericRead<uint16_t>();
        extra_data.AsciiCode = stream.GenericRead<uint8_t>();

        *tile.ExtraTileData.AngelicCountingCloudExtra = std::move(extra_data);
        break;
    }

    case InfinityWeatherMachine: {
        WorldTileInfinityWeatherMachineExtra extra_data{};
        extra_data.IntervalMinutes = stream.GenericRead<uint32_t>();
        extra_data.WeatherMachineList = stream.ReadList<uint32_t, uint32_t>();

        *tile.ExtraTileData.InfinityWeatherMachineExtra = std::move(extra_data);
        break;
    }

    case PineappleGuzzler: {
        WorldTilePineappleGuzzlerExtra extra_data{};
        stream.Advance(4);

        *tile.ExtraTileData.PineappleGuzzlerExtra = std::move(extra_data);
        break;
    }

    case KrakenGalaticBlock: {
        WorldTileKrakenBlockExtra extra_data {};
        extra_data.PatternIndex = stream.GenericRead<uint8_t>();
        extra_data.Unk1 = stream.GenericRead<uint32_t>();

        // this should be in the right order.
        extra_data.R = stream.GenericRead<uint8_t>();
        extra_data.G = stream.GenericRead<uint8_t>();
        extra_data.B = stream.GenericRead<uint8_t>();

        *tile.ExtraTileData.KrakenBlockExtra = std::move(extra_data);
        break;
    }

    case FriendsEntrance: {
        WorldTileFriendsEntranceExtra extra_data {};
        extra_data.OwnerUserId = stream.GenericRead<uint32_t>();
        extra_data.Unk1 = stream.GenericRead<uint16_t>();
        extra_data.AllowedUserList = stream.ReadList<uint16_t, uint32_t>();

        *tile.ExtraTileData.FriendEntranceExtra = std::move(extra_data);
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
