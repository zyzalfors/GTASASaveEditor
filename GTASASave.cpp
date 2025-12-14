#include "GTASASave.h"
#include <cstring>
#include <fstream>

GTASASave::GTASASave(std::string& _path) {
    this->path = _path;

    std::ifstream in(path, std::ios::in | std::ios::binary);
    in.seekg(0, std::ios::end);
    this->size = in.tellg();
    in.seekg(0, std::ios::beg);

    this->bytes = std::unique_ptr<std::uint8_t[]>(new std::uint8_t[this->size]);
    in.read((char*) this->bytes.get(), this->size);
    in.close();

    this->ReadBlockOffsets();
}

void GTASASave::Update(std::string& _name, std::string& _val) {
    if(this->infos.count(_name) == 0) return;
    auto info = this->infos[_name];
    auto type = std::get<0>(info);

    std::size_t offset = this->blockOffsets[std::get<1>(info)] + std::get<2>(info);
    std::uint8_t* buffer = this->bytes.get();
    std::uint8_t intBytes[4];
    std::size_t n = 0;

    switch(type) {
        case boolean: {
                n = 1;
                intBytes[0] = std::stoul(_val) > 0;
            }
            break;
        case byte: {
                n = 1;
                intBytes[0] = std::stoul(_val);
            }
            break;
        case integer: {
                n = 4;
                GetLEBytes(intBytes, std::stoul(_val));
            }
            break;
        case decimal: {
                n = 4;
                std::uint32_t val = 0;
                float floatVal = std::stof(_val);
                std::memcpy(&val, &floatVal, sizeof(float));
                GetLEBytes(intBytes, val);
            }
            break;
    }
    for(std::size_t i = 0; i < n && i < this->size; i++) buffer[offset + i] = intBytes[i];
}

void GTASASave::UpdateChecksum() {
    if(this->size < 4) return;

    std::uint8_t* buffer = this->bytes.get();
    std::uint32_t checksum = 0;
    for(std::size_t i = 0; i < this->size - 4; i++) checksum += buffer[i];

    std::uint8_t checkBytes[4];
    GetLEBytes(checkBytes, checksum);

    for(std::size_t i = 0; i < 4; i++) buffer[this->size - 4 + i] = checkBytes[i];
}

void GTASASave::Write() {
    std::ofstream out(this->path, std::ios::out | std::ios::binary);
    out.write((char*) this->bytes.get(), this->size);
    out.close();
}

void GTASASave::GetInfos(std::map<std::string, bool>& _bools, std::map<std::string, std::uint8_t>& _bytes,
                         std::map<std::string, std::uint32_t>& _ints, std::map<std::string, float>& _floats) {
    bool boolVal = 0;
    std::uint32_t intVal = 0;
    float floatVal = 0;
    std::uint8_t* buffer = this->bytes.get();

    for(auto& info : this->infos) {
        auto infoVal = info.second;
        auto type = std::get<0>(infoVal);
        std::string name = info.first;
        std::size_t offset = this->blockOffsets[std::get<1>(infoVal)] + std::get<2>(infoVal);

        switch(type) {
            case boolean:
                boolVal = buffer[offset] > 0;
                _bools.insert({name, boolVal});
                break;
            case byte:
                intVal = buffer[offset];
                _bytes.insert({name, intVal});
                break;
            case integer:
                intVal = GetInt(buffer, offset);
                _ints.insert({name, intVal});
                break;
            case decimal:
                intVal = GetInt(buffer, offset);
                floatVal = *((float*) &intVal);
                _floats.insert({name, floatVal});
                break;
        }
    }
}

void GTASASave::ReadBlockOffsets() {
    std::uint8_t patt[5] = {'B', 'L', 'O', 'C', 'K'};
    std::uint8_t pi[6] = {0, 0, 0, 0, 0, 0};

    for(std::uint8_t i = 1, m = 0; i <= 5; i++) {
        while(m > 0 && patt[m] != patt[i]) m = pi[m - 1];
        if(patt[m] == patt[i]) m++;
        pi[i] = m;
    }

    std::uint8_t* buffer = this->bytes.get();
    for(std::size_t i = 0, m = 0; i < this->size; i++) {
        while(m > 0 && patt[m] != buffer[i]) m = pi[m - 1];
        if(patt[m] == buffer[i]) m++;
        if(m == 5) {
            this->blockOffsets.push_back(i + 1);
            m = pi[m - 1];
        }
    }
}

void GTASASave::GetLEBytes(std::uint8_t _buff[], std::uint32_t _val) {
    _buff[0] = _val & 0xFF;
    _buff[1] = (_val & 0xFF00) >> 8;
    _buff[2] = (_val & 0xFF0000) >> 16;
    _buff[3] = (_val & 0xFF000000) >> 24;
}

std::uint32_t GTASASave::GetInt(std::uint8_t _buff[], std::size_t _offset) {
    return (_buff[_offset + 3] << 24) | (_buff[_offset + 2] << 16) | (_buff[_offset + 1] << 8) | _buff[_offset];
}

