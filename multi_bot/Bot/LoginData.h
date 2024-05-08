#pragma once
#include <unordered_map>
#include <string>
#include <memory>
struct LoginData 
{
    std::string GrowID{};
    std::string GrowIDPass{};
    std::string GameProtocol = "205";
    std::string GameVersion = "4.55";
    std::string Meta{};
    std::string Mac{};
    std::string Wk{};
    std::string Hash{};
    std::string Rid{};
};

