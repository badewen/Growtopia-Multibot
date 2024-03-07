#pragma once

#include <string>

struct LoginData 
{
    std::string GrowID{};
    std::string GrowIDPass{};
    std::string GameProtocol = "205";
    std::string GameVersion = "4.51";
    std::string Meta{};
    std::string Mac{};
    std::string Wk{};
    std::string Hash{};
    std::string Rid{};
};