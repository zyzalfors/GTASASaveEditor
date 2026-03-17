#include "GTASASave.h"
#include <cstring>
#include <fstream>

GTASASave::GTASASave(const std::string& path) {
    this->path = path;

    std::ifstream in(path, std::ios::in | std::ios::binary);
    in.seekg(0, std::ios::end);
    const std::size_t s = in.tellg();
    in.seekg(0, std::ios::beg);

    if(s == size) {
        this->bytes = std::unique_ptr<std::uint8_t[]>(new std::uint8_t[size]);
        in.read((char*) this->bytes.get(), size);
        in.close();
    }
    else {
        in.close();
        throw std::runtime_error("Invalid save size.");
    }

    this->ReadBlockOffsets();
}

void GTASASave::Update(const std::string& name, const std::string& val) {
    if(this->infos.count(name) == 0) return;

    const auto info = this->infos[name];
    const Type type = std::get<0>(info);
    const std::size_t offset = this->blockOffsets[std::get<1>(info)] + std::get<2>(info);

    std::uint8_t* buffer = this->bytes.get();
    std::uint8_t bytes[4];
    std::size_t n = 0;

    switch(type) {
        case boolean:
            n = 1;
            bytes[0] = std::stoul(val) > 0;
            break;

        case byte:
            n = 1;
            bytes[0] = std::stoul(val);
            break;

        case integer:
            n = 4;
            GetLEBytes(bytes, std::stoul(val));
            break;

        case decimal: {
                n = 4;
                const float floatVal = std::stof(val);
                std::uint32_t intVal = 0;
                std::memcpy(&intVal, &floatVal, 4);
                GetLEBytes(bytes, intVal);
            }
            break;
    }

    for(std::size_t i = 0; i < n && i < this->size; i++)
        buffer[offset + i] = bytes[i];
}

void GTASASave::UpdateChecksum() {
    if(this->size < 4) return;

    std::uint8_t* buffer = this->bytes.get();
    std::uint32_t checksum = 0;

    for(std::size_t i = 0; i < this->size - 4; i++)
        checksum += buffer[i];

    std::uint8_t bytes[4];
    GetLEBytes(bytes, checksum);

    for(std::size_t i = 0; i < 4; i++)
        buffer[this->size - 4 + i] = bytes[i];
}

bool GTASASave::CheckChecksum() {
    if(this->size < 4) return false;

    const std::uint8_t* buffer = this->bytes.get();
    std::uint32_t checksum = 0;

    for(std::size_t i = 0; i < this->size - 4; i++)
        checksum += buffer[i];

    const std::uint32_t calcChecksum = GetInt(buffer, this->size - 4);
    return checksum == calcChecksum;
}

void GTASASave::Write() {
    std::ofstream out(this->path, std::ios::out | std::ios::binary);
    out.write((char*) this->bytes.get(), this->size);
    out.close();
}

void GTASASave::GetInfos(std::string& path, std::map<std::string, bool>& bools, std::map<std::string, std::uint8_t>& bytes, std::map<std::string, std::uint32_t>& ints, std::map<std::string, float>& floats) {
    path = this->path;
    const std::uint8_t* buffer = this->bytes.get();

    for(const auto& info : this->infos) {
        const std::string name = info.first;
        const auto val = info.second;
        const Type type = std::get<0>(val);
        const std::size_t offset = this->blockOffsets[std::get<1>(val)] + std::get<2>(val);

        switch(type) {
            case boolean:
                bools.insert({name, buffer[offset] > 0});
                break;

            case byte:
                bytes.insert({name, buffer[offset]});
                break;

            case integer:
                ints.insert({name, GetInt(buffer, offset)});
                break;

            case decimal: {
                    const std::uint32_t intVal = GetInt(buffer, offset);
                    float floatVal = 0;
                    std::memcpy(&floatVal, &intVal, 4);
                    floats.insert({name, floatVal});
                }
                break;
        }
    }
}

void GTASASave::ReadBlockOffsets() {
    const std::uint8_t patt[5] = {'B', 'L', 'O', 'C', 'K'};
    std::uint8_t pi[5] = {0, 0, 0, 0, 0};

    for(std::uint8_t i = 1, m = 0; i < 5; i++) {
        while(m > 0 && patt[m] != patt[i])
            m = pi[m - 1];

        if(patt[m] == patt[i]) m++;
        pi[i] = m;
    }

    const std::uint8_t* buffer = this->bytes.get();

    for(std::size_t i = 0, m = 0; i < this->size; i++) {
        while(m > 0 && patt[m] != buffer[i])
            m = pi[m - 1];

        if(patt[m] == buffer[i]) m++;

        if(m == 5) {
            this->blockOffsets.push_back(i + 1);
            m = pi[m - 1];
        }
    }
}

void GTASASave::GetLEBytes(std::uint8_t buffer[], const std::uint32_t& val) {
    buffer[0] = val & 0xFF;
    buffer[1] = (val & 0xFF00) >> 8;
    buffer[2] = (val & 0xFF0000) >> 16;
    buffer[3] = (val & 0xFF000000) >> 24;
}

std::uint32_t GTASASave::GetInt(const std::uint8_t buffer[], const std::size_t& offset) {
    return (buffer[offset + 3] << 24) | (buffer[offset + 2] << 16) | (buffer[offset + 1] << 8) | buffer[offset];
}
