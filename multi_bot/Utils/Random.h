#pragma once

#include <random>

namespace Utils {
    class Random
    {
    public:
        static uint64_t RandomUInt64();
        static std::string RandomHex(size_t length, bool upper);

        static std::string RandomMac();
    };
}
