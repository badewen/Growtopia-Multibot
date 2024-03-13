#pragma once

#include <vector>
#include <unordered_map>
#include <optional>

#include "../../Utils/BinaryStreamReader.h"
#include "WorldItemObject.h"
#include "WorldTile.h"

class WorldInfo
{
public:
    bool SerializeFromMem(std::vector<uint8_t> world_data_raw, std::optional<WorldTile>& faulty_tile);

    void Reset();

    void AddItemObject(uint32_t item_id, float pos_x, float pos_y, uint32_t amount);
    void RemoveItemObject(WorldItemObjectID obj_id);

    WorldItemObject GetItemObject(WorldItemObjectID obj_id);
    std::vector<WorldItemObject> GetItemObjects();

    void SetTile(uint32_t x, uint32_t y, WorldTile new_tile);
    const WorldTile& GetTileRef(uint32_t x, uint32_t y);

    const std::vector<WorldTile>& GetTilesRef() { return m_tiles; }

    bool IsParsedSuccessfully() { return m_parsed_succesfully; }

    static bool ParseTile(Utils::BinaryStreamReader& stream, WorldTile& tile);
    static bool ParseExtraTileData(WorldTile& tile, Utils::BinaryStreamReader& stream);

private:

    void parse_item_drops(Utils::BinaryStreamReader& stream);

private:
    uint32_t m_width, m_height;
    uint32_t m_last_obj_id;
    uint16_t m_base_weather, m_current_weather;
    
    bool m_parsed_succesfully;

    std::string m_world_name;
    std::vector<WorldTile> m_tiles{};
    std::unordered_map<WorldItemObjectID, WorldItemObject> m_dropped_items;
};

