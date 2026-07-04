#include "GTASASave.h"
#include <cstring>
#include <fstream>

GTASASave::GTASASave(const std::string& path) {
    this->path = path;

    std::ifstream in(path, std::ios::in | std::ios::binary);
    in.seekg(0, std::ios::end);
    const std::size_t s = in.tellg();
    in.seekg(0, std::ios::beg);

    if(s == saveSize) {
        in.read((char*) this->bytes.data(), saveSize);
        in.close();
    }
    else {
        in.close();
        throw std::runtime_error("Invalid save size.");
    }

    this->ReadBlockOffsets();
}

void GTASASave::UpdateValue(const std::string& name, const std::string& val) {
    if(this->values.count(name) == 0) return;

    const auto value = this->values[name];
    const Type type = std::get<0>(value);
    const std::size_t offset = this->blockOffsets[std::get<1>(value)] + std::get<2>(value);

    std::uint8_t* buffer = this->bytes.data();
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
                const float decVal = std::stof(val);
                std::uint32_t intVal = 0;
                std::memcpy(&intVal, &decVal, 4);
                GetLEBytes(bytes, intVal);
            }
            break;
    }

    std::memcpy(buffer + offset, bytes, n);
}

void GTASASave::UpdateWeapons(std::array<std::pair<std::string, std::uint32_t>, weaponSlots>& weaps) {
    std::uint8_t* buffer = this->bytes.data();
    const std::size_t offset = this->blockOffsets[2] + 0x28;
    std::uint8_t bytes[4];

    for(std::size_t i = 0; i < weaponSlots; i++) {
        auto weapon = weaps[i];
        std::uint32_t weaponId = 0;
        std::uint32_t ammo = (i < 2 || i > 9) ? 1 : weapon.second;

        for(const auto& entry : weapons[i])
            if(entry.first == weapon.first) weaponId = entry.second;

        GetLEBytes(bytes, weaponId);
        std::memcpy(buffer + offset + i * weaponSize, bytes, 4);

        GetLEBytes(bytes, ammo);
        std::memcpy(buffer + offset + i * weaponSize + 0x0C, bytes, 4);
    }
}

bool GTASASave::ValidChecksum() {
    const std::uint8_t* buffer = this->bytes.data();
    std::uint32_t checksum = 0;

    for(std::size_t i = 0; i < checksumOffset; i++)
        checksum += buffer[i];

    const std::uint32_t calcChecksum = GetInt(buffer, checksumOffset);
    return checksum == calcChecksum;
}

void GTASASave::Write() {
    std::uint8_t* buffer = this->bytes.data();
    std::uint32_t checksum = 0;

    for(std::size_t i = 0; i < checksumOffset; i++)
        checksum += buffer[i];

    std::uint8_t bytes[4];
    GetLEBytes(bytes, checksum);
    std::memcpy(buffer + checksumOffset, bytes, 4);

    std::ofstream out(this->path, std::ios::out | std::ios::binary);
    out.write((char*) this->bytes.data(), saveSize);
    out.close();
}

void GTASASave::GetInfos(std::string& path, std::map<std::string, bool>& bools,
                         std::map<std::string, std::uint8_t>& bytes, std::map<std::string, std::uint32_t>& ints,
                         std::map<std::string, float>& decs, std::array<std::pair<std::string, std::uint32_t>, weaponSlots>& weaps) {
    path = this->path;
    const std::uint8_t* buffer = this->bytes.data();

    for(const auto& value : this->values) {
        const std::string name = value.first;
        const auto val = value.second;
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
                    float decVal = 0;
                    std::memcpy(&decVal, &intVal, 4);
                    decs.insert({name, decVal});
                }
                break;
        }
    }

    const std::size_t offset = this->blockOffsets[2] + 0x28;

    for(std::size_t i = 0; i < weaponSlots; i++) {
        const std::uint32_t weaponId = GetInt(buffer, offset + i * weaponSize);
        std::string weapon;

        for(const auto& entry : weapons[i])
            if(entry.second == weaponId) weapon = entry.first;

        const std::uint32_t ammo = GetInt(buffer, offset + i * weaponSize + 0x0C);
        weaps[i] = std::make_pair(weapon, ammo);
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

    const std::uint8_t* buffer = this->bytes.data();

    for(std::size_t i = 0, m = 0; i < saveSize; i++) {
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
