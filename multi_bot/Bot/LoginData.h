#pragma once

#include <string>

struct LoginData 
{
    std::string growid{};
    std::string growid_pass{};
    std::string game_protocol = "205";
    std::string game_version = "4.5";
    std::string meta{};
    std::string mac{};
    std::string wk{};
    std::string hash{};
    std::string rid{};
};