// from https://github.com/ZTzTopia/GTProxy/blob/develop/src/utils/hash.h

#pragma once

#include <string_view>
#include <algorithm>
#include <array>

#include <fmt/fmt.h>
#include <openssl/evp.h>
#include <openssl/sha.h>

namespace Utils {
    namespace Hash {
        static constexpr std::size_t fnv1a(const std::string_view& data) {
            // Fowler/Noll/Vo 1a variant.
            std::size_t prime{ 16777619U };
            std::size_t offset_basis{ 2166136261U };

            if constexpr (sizeof(std::size_t) == 8) {
                // 64-bit
                prime = 1099511628211ULL;
                offset_basis = 14695981039346656037ULL;
            }

            std::size_t hash{ offset_basis };
            for (auto& c : data) {
                hash ^= c;
                hash *= prime;
            }

            return hash;
        }

        static constexpr std::int32_t proton(const char* data, std::size_t length = 0) {
            std::int32_t hash{ 0x55555555 };
            if (data) {
                if (length > 0) {
                    while (length--) {
                        hash = (hash >> 27) + (hash << 5) + *reinterpret_cast<const std::uint8_t*>(data++);
                    }

                    return hash;
                }

                while (*data) {
                    hash = (hash >> 27) + (hash << 5) + *reinterpret_cast<const std::uint8_t*>(data++);
                }
            }

            return hash;
        }

        // copied from https://github.com/Nuron-bit/KLV-Generator
        static std::string sha256(const std::string& input) {
            std::array<unsigned char, SHA256_DIGEST_LENGTH> digest{};
           
            SHA256((const unsigned char*)input.data(), input.length(), digest.data());

            std::string sha256{};

            sha256.reserve(SHA256_DIGEST_LENGTH * 2);
            for (int i{ 0 }; i < SHA256_DIGEST_LENGTH; i++) {
                sha256 += fmt::format("{:02x}", digest[i]);
            }
            return sha256;
        }

        // copied from https://github.com/Nuron-bit/KLV-Generator
        static std::string md5(const std::string& input) {
            unsigned char digest[EVP_MAX_MD_SIZE];
            unsigned int digest_len;

            EVP_MD_CTX* ctx = EVP_MD_CTX_new();
            EVP_DigestInit_ex(ctx, EVP_md5(), nullptr);
            EVP_DigestUpdate(ctx, input.c_str(), input.length());
            EVP_DigestFinal_ex(ctx, digest, &digest_len);
            EVP_MD_CTX_free(ctx);

            char md5string[33];
            for (int i = 0; i < 16; i++) {
                sprintf(&md5string[i * 2], "%02x", (unsigned int)digest[i]);
            }
            md5string[32] = '\0';

            std::string hash = std::string(md5string);
            std::transform(hash.begin(), hash.end(), hash.begin(), ::toupper);
            return hash;
        }
    }
}

constexpr size_t operator "" _fh(const char* left, size_t len) {
    return Utils::Hash::fnv1a(std::string_view{ left, len });
}
