#ifndef GTASASAVE_H
#define GTASASAVE_H
#include <vector>
#include <array>
#include <tuple>
#include <map>
#include <string>
#include <cstdint>

class GTASASave {
    public: static const std::size_t weaponSlots = 13;
    private: static const std::size_t saveSize = 202752;
    private: static const std::size_t checksumOffset = 202748;
    private: std::array<std::uint8_t, saveSize> bytes;
    private: std::vector<std::size_t> blockOffsets;
    private: std::string path;

    public: enum Type {boolean, byte, integer, decimal};
    private: std::map<std::string, std::tuple<Type, std::size_t, std::size_t>> values = {
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
            {"Roadblocked SF", std::make_tuple(boolean, 21, 0x56)},
            {"Roadblocked LV", std::make_tuple(boolean, 21, 0x57)},
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

    public: std::array<std::map<std::string, std::uint32_t>, weaponSlots> weapons = {{
            {{"None", 0x00}, {"Brass knuckles", 0x01}},
            {{"None", 0x00}, {"Golf club", 0x02}, {"Nightstick", 0x03}, {"Knife", 0x04}, {"Baseball bat", 0x05}, {"Shovel", 0x06}, {"Pool cue", 0x07}, {"Katana", 0x08}, {"Chainsaw", 0x09}},
            {{"None", 0x00}, {"Pistol", 0x16}, {"Silenced pistol", 0x17}, {"Desert Eagle", 0x18}},
            {{"None", 0x00}, {"Shotgun", 0x19}, {"Sawn-off shotgun", 0x1A}, {"Combat shotgun", 0x1B}},
            {{"None", 0x00}, {"Tec-9", 0x20}, {"Micro SMG", 0x1C}, {"SMG", 0x1D}},
            {{"None", 0x00}, {"AK47", 0x1E}, {"M4", 0x1F}},
            {{"None", 0x00}, {"Rifle", 0x21}, {"Sniper rifle", 0x22}},
            {{"None", 0x00}, {"Rocket launcher", 0x23}, {"Heat seeking rocket launcher", 0x24}, {"Flame thrower", 0x25}, {"Minigun", 0x26}},
            {{"None", 0x00}, {"Grenade", 0x10}, {"Tear gas", 0x11}, {"Molotov", 0x12}, {"Remote explosives", 0x27}},
            {{"None", 0x00}, {"Spraycan", 0x29}, {"Fire extinguisher", 0x2A}, {"Camera", 0x2B}},
            {{"None", 0x00}, {"Dildo 1", 0x0A}, {"Dildo 2", 0x0B}, {"Vibrator 1", 0x0C}, {"Vibrator 2", 0x0D}, {"Flowers", 0x0E}, {"Cane", 0x0F}},
            {{"None", 0x00}, {"Night-vision goggles", 0x2C}, {"Thermal goggles", 0x2D}, {"Parachute", 0x2E}},
            {{"None", 0x00}, {"Remote detonator", 0x28}}}
    };
    private: static const std::size_t weaponSize = 28;

    public: GTASASave(const std::string& path);
    public: void UpdateValue(const std::string& name, const std::string& val);
    public: void UpdateWeapons(std::array<std::pair<std::string, std::uint32_t>, weaponSlots>& weaps);
    public: void Update();
    public: bool ValidChecksum();
    public: void Write();
    public: void GetInfos(std::string& path, std::map<std::string, bool>& bools,
                          std::map<std::string, std::uint8_t>& bytes, std::map<std::string, std::uint32_t>& ints,
                          std::map<std::string, float>& decs, std::array<std::pair<std::string, std::uint32_t>, weaponSlots>& weaps);

    private: void ReadBlockOffsets();
    private: static void GetLEBytes(std::uint8_t buff[], const std::uint32_t& val);
    private: static std::uint32_t GetInt(const std::uint8_t buffer[], const std::size_t& offset);
};
#endif
