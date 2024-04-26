#pragma once

#include <vector>
#include <unordered_map>
#include <optional>

#include "../../ItemDatabase/ItemDatabase.h"
#include "../../Utils/BinaryStreamReader.h"
#include "WorldItemObject.h"
#include "WorldTile.h"
#include "WorldNPC.h"


class WorldInfo
{
public:
    WorldInfo(std::shared_ptr<ItemDatabase> items_dat) :
        m_items_dat{ std::move(items_dat) }
    {
    }

    bool SerializeFromMem(std::vector<uint8_t> world_data_raw, std::optional<WorldTile>& faulty_tile);

    void Reset();

    void AddItemObject(uint32_t item_id, float pos_x, float pos_y, uint32_t amount);
    void RemoveItemObject(WorldItemObjectID obj_id);

    WorldItemObject GetItemObject(WorldItemObjectID obj_id);
    std::vector<WorldItemObject> GetItemObjects();

    void SetNPC(WorldNPC npc_data);
    void RemoveNPC(WorldNPCIndex npc_index);

    std::optional<WorldNPC> GetNPC(WorldNPCIndex npc_index);
    std::vector<WorldNPC> GetNPCs();

    // this is used when trying to get the current npc pos.
    void UpdateNPCsPos();

    void SetTile(uint32_t x, uint32_t y, WorldTile new_tile);
    const WorldTile& GetTileRef(uint32_t x, uint32_t y);

    const std::vector<WorldTile>& GetTilesRef() { return m_tiles; }
    const WorldTile GetFaultyTile() { return m_faulty_tile; }

    bool IsParsedSuccessfully() { return m_parsed_succesfully; }

    static bool ParseTile(
        Utils::BinaryStreamReader& stream,
        WorldTile& tile,
        std::shared_ptr<ItemDatabase> items_dat
    );
    static bool ParseExtraTileData(
        WorldTile& tile, 
        Utils::BinaryStreamReader& stream, 
        std::shared_ptr<ItemDatabase> items_dat
    );

private:
    void parse_item_drops(Utils::BinaryStreamReader& stream);

private:
    uint32_t m_width{}, m_height{};
    uint32_t m_last_obj_id{};
    uint16_t m_base_weather{}, m_current_weather{};
    
    bool m_parsed_succesfully{};

    WorldTile m_faulty_tile{};
    
    std::shared_ptr<ItemDatabase> m_items_dat{};
    std::string m_world_name{};
    std::vector<WorldTile> m_tiles{};
    std::unordered_map<WorldNPCIndex, WorldNPC> m_npc_list{};
    std::unordered_map<WorldItemObjectID, WorldItemObject> m_dropped_items{};
};

