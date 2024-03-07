#pragma once

#include <vector>
#include <string>

namespace Utils {
    class BinaryStreamReader
    {
    public:
        BinaryStreamReader(uint8_t* buffer, size_t buffer_size);
        BinaryStreamReader(std::vector<uint8_t> buffer);

        void ResetPos();
        void Advance(size_t off);

        uint8_t ReadByte();
        std::vector<uint8_t> ReadBytes(size_t count);

        float ReadFloat();

        std::string ReadString();

        template <typename T>
        T GenericRead() {
            T ret = *((T*)ReadBytes(sizeof(T)).data());

            return ret;
        }

        template <typename T>
        std::vector<T> GenericReadMultiple(size_t count) {
            std::vector<T> ret = {};
            for (size_t i = 0; i < count; i++) {
                ret.push_back(GenericRead<T>());
            }

            return ret;
        }

        // for these read list functions, the list length MUST be at the buffer pointer

        // LenType = the array/list size in byte. Because the list length is specified at the beginning of the array/list.
        // ElmType = the array/list element type
        template <typename ListLenType, typename ElmType>
        std::vector<ElmType> ReadList() {
            ListLenType list_length = GenericRead<ListLenType>();

            return GenericReadMultiple<ElmType>(list_length);
        }

    private:
        std::vector<uint8_t> m_buffer{};
        std::vector<uint8_t>::iterator m_curr_it{};
    };
}
