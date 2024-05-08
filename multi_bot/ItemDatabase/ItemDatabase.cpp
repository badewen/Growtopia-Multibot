#include "ItemDatabase.h"

#include <fstream>

#include "../Utils/BinaryStreamReader.h"

bool ItemDatabase::LoadFromFile(const std::string& file_name) {
    std::ifstream input_file{file_name, std::ios::in | std::ios::binary | std::ios::ate };
    
    if (!input_file.is_open() || input_file.fail()) {
        return false;
    }

    size_t buffer_size = input_file.tellg();
    uint8_t* buffer = (uint8_t*)malloc(input_file.tellg());
    
    if (!buffer) {
        return false;
    }

    input_file.seekg(std::ios::beg);
    input_file.read((char*)buffer, buffer_size);
    input_file.close();

    Utils::BinaryStreamReader reader = { buffer, buffer_size };

    LoadFromReader(reader);

    free(buffer);
    return true;
}

void ItemDatabase::LoadFromReader(Utils::BinaryStreamReader& reader) {
    m_items_dat_version = reader.GenericRead<uint16_t>();
    m_items_count = reader.GenericRead<uint32_t>();

    m_item_info_map.reserve(m_items_count);

    for (uint32_t i = 0; i < m_items_count; i++) {
        auto item_info = parse_next_item_info(reader);
        m_item_info_map.insert_or_assign(item_info.ItemID, item_info);
    }
}

std::string ItemDatabase::DecryptString(ItemID item_id, const std::vector<uint8_t>& buffer) {
    std::string cypher = "PBG892FXX982ABC*";
    std::string ret{};

    for (size_t i = 0; i < buffer.size(); i++) {
        size_t cypher_char_index = (i + item_id) % cypher.length();
        char cypher_char = cypher[cypher_char_index];

        ret.push_back(buffer[i] ^ cypher_char);
    }

    return ret;
}

ItemInfo ItemDatabase::GetItemInfo(uint32_t item_id) {
    auto it = m_item_info_map.find(item_id);

    return it == m_item_info_map.end() ? ItemInfo{} : std::move(it->second);
}

ItemInfo ItemDatabase::parse_next_item_info(Utils::BinaryStreamReader& reader) {
    ItemInfo ret{};

    ret.ItemID = reader.GenericRead<uint32_t>();
    ret.Flags = reader.GenericRead<ItemInfoFlag>();
    ret.ItemType = reader.GenericRead<eItemInfoItemType>();
    ret.MaterialType = reader.GenericRead<eItemInfoMaterialType>();
    ret.ItemName = DecryptString(ret.ItemID, reader.ReadList<uint16_t, uint8_t>());
    ret.TextureFilePath = reader.ReadString();
    ret.TextureFileHash = reader.GenericRead<uint32_t>();
    ret.VisualEffect = reader.GenericRead<eItemInfoVisualEffect>();
    ret.CookingIngredientType = reader.GenericRead<uint32_t>();
    ret.TextureCoordX = reader.GenericRead<uint8_t>();
    ret.TextureCoordY = reader.GenericRead<uint8_t>();
    ret.TextureType = reader.GenericRead<eItemInfoTextureType>();
    ret.IsStripeyWallpaper = reader.GenericRead<uint8_t>();
    ret.CollisionType = reader.GenericRead<eItemInfoCollisionType>();
    ret.Health = reader.GenericRead<uint8_t>();
    ret.ResetTime = reader.GenericRead<uint32_t>();
    ret.ClothingType = reader.GenericRead<eItemInfoClothingType>();
    ret.Rarity = reader.GenericRead<uint16_t>();
    ret.MaxAmount = reader.GenericRead<uint8_t>();
    ret.ExtraFile = reader.ReadString();
    ret.ExtraFileHash = reader.GenericRead<uint32_t>();
    ret.AnimationTime = reader.GenericRead<uint32_t>();
    ret.PetName = reader.ReadString();
    ret.PetPrefix = reader.ReadString();
    ret.PetSuffix = reader.ReadString();
    ret.PetAbility = reader.ReadString();
    ret.SeedBase = reader.GenericRead<eItemInfoSeedBase>();
    ret.SeedOverlay = reader.GenericRead<eItemInfoSeedOverlay>();
    ret.TreeBase = reader.GenericRead<eItemInfoTreeBase>();
    ret.TreeLeaves = reader.GenericRead<eItemInfoTreeLeaves>();
    ret.SeedColor = reader.GenericRead<eItemInfoColor>();
    ret.SeedOverlayColor = reader.GenericRead<eItemInfoColor>();
    ret.Ingredient = reader.GenericRead<uint32_t>();
    ret.GrowTime = reader.GenericRead<uint32_t>();
    ret.Flags2 = reader.GenericRead<ItemInfoFlag2>();
    ret.IsRayman = reader.GenericRead<uint16_t>();
    ret.ExtraOption = reader.ReadString();
    ret.TextureFile2 = reader.ReadString();
    ret.ExtraOption2 = reader.ReadString();

    reader.Advance(8);

    ret.Flags3 = reader.GenericRead<ItemInfoFlag3>();

    reader.Advance(68);

    ret.PunchOptions = reader.ReadString();
    reader.Advance(4);

    reader.ReadBytesToBuffer(ret.BodyPartList, 9);

    reader.Advance(33);
    reader.ReadString();

    ret.RendererFilePath = reader.ReadString();

    reader.GenericRead<uint32_t>();

    ret.RendererFileHash = reader.GenericRead<int32_t>();

    return ret;
}


