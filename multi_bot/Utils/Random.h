#pragma once

#include <random>

namespace Utils {
    class Random
    {
    public:
        static uint64_t RandomUInt64();
        static uint64_t RandomUInt64(uint64_t min, uint64_t max);
        static std::string RandomHex(size_t length, bool upper);

        static std::string RandomMac();
    };
}
