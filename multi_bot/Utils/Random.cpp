#include "Random.h"

#include <fmt/fmt.h>

using namespace Utils;

std::random_device hw_rand {};
std::mt19937_64 sw_mt_rand {hw_rand()};

uint64_t Random::RandomUInt64() {
    return sw_mt_rand();
}

std::string Random::RandomHex(size_t length, bool upper) {
    std::string chars = upper ? "1234567890ABCDEF" : "1234567890abcdef";
    
    std::string ret;

    for (size_t i = 0; i < length; i++) {
        ret.push_back(chars[RandomUInt64() % chars.length()]);
    }

    return ret;
}

std::string Random::RandomMac() {
    std::string ret;

    for (size_t i = 0; i < 12; i++) {
        ret += RandomHex(2, true) + ":";
    }

    // remove trailing colon
    ret.pop_back();

    return ret;
}
