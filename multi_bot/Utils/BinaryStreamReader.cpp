#include "BinaryStreamReader.h"

using namespace Utils;

BinaryStreamReader::BinaryStreamReader(uint8_t* buffer, size_t buffer_size) {
    std::vector<uint8_t> buffer_vec{};

    buffer_vec.insert(buffer_vec.end(), buffer, buffer + buffer_size);

    m_buffer = std::move(buffer_vec);
    m_curr_it = m_buffer.begin();
}

BinaryStreamReader::BinaryStreamReader(std::vector<uint8_t> buffer) {
    m_buffer = std::move(buffer);
    m_curr_it = m_buffer.begin();
}

void BinaryStreamReader::ResetPos() {
    m_curr_it = m_buffer.begin();
}

void BinaryStreamReader::Advance(size_t off) {
    std::advance(m_curr_it, off);
}

uint8_t BinaryStreamReader::ReadByte() {
    uint8_t ret = *m_curr_it;
    std::advance(m_curr_it, sizeof(uint8_t));

    return ret;
}

std::vector<uint8_t> BinaryStreamReader::ReadBytes(size_t count) {
    std::vector<uint8_t> ret = {};

    for (size_t i = 0; i < count; i++) {
        ret.push_back(ReadByte());
    }

    return ret;
}

void Utils::BinaryStreamReader::ReadBytesToBuffer(uint8_t* out_buffer, size_t count) {
    auto bytes = ReadBytes(count);
    memcpy(out_buffer, bytes.data(), count);
}

float BinaryStreamReader::ReadFloat() {
    return GenericRead<float>();
}

std::string BinaryStreamReader::ReadString() {
    std::vector<char> str_raw = ReadList<uint16_t, char>();

    return std::string{ str_raw.begin(), str_raw.end() };
}
