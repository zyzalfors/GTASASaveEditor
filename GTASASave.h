#ifndef GTASASAVE_H
#define GTASASAVE_H
#include <vector>
#include <tuple>
#include <map>
#include <string>
#include <memory>
#include <cstdint>

class GTASASave {
    public: enum Type {boolean, byte, integer, decimal};
    public: GTASASave(std::string& _path);
    public: void Update(std::string& _name, std::string& _val);
    public: void UpdateChecksum();
    public: void Write();
    public: void GetInfos(std::map<std::string, bool>& _bools, std::map<std::string, uint8_t>& _bytes, std::map<std::string, uint32_t>& _ints, std::map<std::string, float>& _floats);
    private: std::unique_ptr<uint8_t[]> bytes;
    private: size_t size = 0;
    private: std::vector<size_t> blockOffsets;
    private: std::string path;
    private: std::map<std::string, std::tuple<Type, size_t, size_t>> infos = {
            {"Cheated", std::make_tuple(boolean, 0, 0x90)},
            {"Riot", std::make_tuple(boolean, 0, 0xE0)},
            {"Uncensored", std::make_tuple(boolean, 0, 0xEE)},
            {"Taxi nitro", std::make_tuple(boolean, 0, 0x145)},
            {"Prostitutes pay", std::make_tuple(boolean, 0, 0x146)},
            {"Free bomb", std::make_tuple(boolean, 3, 0x04)},
            {"Free respray", std::make_tuple(boolean, 3, 0x05)},
            {"Respray disabled", std::make_tuple(boolean, 3, 0x06)},
            {"Lose stuff on wasted", std::make_tuple(boolean, 4, 0x04)},
            {"Lose stuff on busted", std::make_tuple(boolean, 4, 0x05)},
            {"Infinite sprint", std::make_tuple(boolean, 15, 0x20)},
            {"Fast reload", std::make_tuple(boolean, 15, 0x21)},
            {"Fireproof", std::make_tuple(boolean, 15, 0x22)},
            {"Drive-by", std::make_tuple(boolean, 15, 0x27)},
            {"Version", std::make_tuple(integer, 0, 0x00)},
            {"Health", std::make_tuple(decimal, 2, 0x20)},
            {"Max health", std::make_tuple(decimal, 16, 0x60)},
            {"Armor", std::make_tuple(decimal, 2, 0x24)},
            {"Max armor", std::make_tuple(byte, 15, 0x24)},
            {"Money", std::make_tuple(integer, 15, 0x04)},
            {"Money on screen", std::make_tuple(integer, 15, 0x10)},
            {"Fat", std::make_tuple(decimal, 16, 0x54)},
            {"Stamina", std::make_tuple(decimal, 16, 0x58)},
            {"Muscle", std::make_tuple(decimal, 16, 0x5C)},
            {"Respect", std::make_tuple(decimal, 16, 0x100)},
            {"Sex appeal", std::make_tuple(decimal, 16, 0x140)},
            {"Luck", std::make_tuple(integer, 16, 0x30C)},
            {"Pistol skill", std::make_tuple(decimal, 16, 0x114)},
            {"Silenced pistol skill", std::make_tuple(decimal, 16, 0x118)},
            {"Desert Eagle skill", std::make_tuple(decimal, 16, 0x11C)},
            {"Shotgun skill", std::make_tuple(decimal, 16, 0x120)},
            {"Sawn-off shotgun skill", std::make_tuple(decimal, 16, 0x124)},
            {"Combat shotgun skill", std::make_tuple(decimal, 16, 0x128)},
            {"Machine pistol skill", std::make_tuple(decimal, 16, 0x12C)},
            {"SMG skill", std::make_tuple(decimal, 16, 0x130)},
            {"AK47 skill", std::make_tuple(decimal, 16, 0x134)},
            {"M4 skill", std::make_tuple(decimal, 16, 0x138)},
            {"Gambling skill", std::make_tuple(decimal, 16, 0x144)},
            {"Driving skill", std::make_tuple(integer, 16, 0x1E8)},
            {"Flying skill", std::make_tuple(integer, 16, 0x2E4)},
            {"Lung capacity", std::make_tuple(integer, 16, 0x2EC)},
            {"Bike skill", std::make_tuple(integer, 16, 0x2FC)},
            {"Cycling skill", std::make_tuple(integer, 16, 0x300)},
            {"Cheats used", std::make_tuple(integer, 16, 0x18C)}
    };
    private: void ReadBlockOffsets();
    private: static void GetLEBytes(uint8_t _buff[], uint32_t _val);
    private: static uint32_t GetInt(uint8_t _buff[], size_t _offset);
};
#endif
