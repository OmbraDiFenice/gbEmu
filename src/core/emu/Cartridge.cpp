#include <Pch.h>

#include "Cartridge.h"

#include <core/emu/utils.h>

Cartridge::~Cartridge() { delete[] _data; }

bool Cartridge::load(const std::string& filename) {
    _data = loadData(filename.c_str());
    return _data != nullptr;
}

std::string Cartridge::getTitle() {
    return std::string{reinterpret_cast<char*>(&_data[0x134]), 15};
}

std::string Cartridge::getCartType() {
    static std::string map[0x100];
    map[0x00] = "ROM ONLY 13h MBC3+RAM+BATTERY";
    map[0x01] = "MBC1 15h MBC4";
    map[0x02] = "MBC1+RAM 16h MBC4+RAM";
    map[0x03] = "MBC1+RAM+BATTERY 17h MBC4+RAM+BATTERY";
    map[0x05] = "MBC2 19h MBC5";
    map[0x06] = "MBC2+BATTERY 1Ah MBC5+RAM";
    map[0x08] = "ROM+RAM 1Bh MBC5+RAM+BATTERY";
    map[0x09] = "ROM+RAM+BATTERY 1Ch MBC5+RUMBLE";
    map[0x0B] = "MMM01 1Dh MBC5+RUMBLE+RAM";
    map[0x0C] = "MMM01+RAM 1Eh MBC5+RUMBLE+RAM+BATTERY";
    map[0x0D] = "MMM01+RAM+BATTERY FCh POCKET CAMERA";
    map[0x0F] = "MBC3+TIMER+BATTERY FDh BANDAI TAMA5";
    map[0x10] = "MBC3+TIMER+RAM+BATTERY FEh HuC3";
    map[0x11] = "MBC3 FFh HuC1+RAM+BATTERY";
    map[0x12] = "MBC3+RAM";
    map[0x13] = "MBC3+RAM+BATTERY";
    map[0x15] = "MBC4";
    map[0x16] = "MBC4+RAM";
    map[0x17] = "MBC4+RAM+BATTERY";
    map[0x19] = "MBC5";
    map[0x1A] = "MBC5+RAM";
    map[0x1B] = "MBC5+RAM+BATTERY";
    map[0x1C] = "MBC5+RUMBLE";
    map[0x1D] = "MBC5+RUMBLE+RAM";
    map[0x1E] = "MBC5+RUMBLE+RAM+BATTERY";
    map[0xFC] = "POCKET CAMERA";
    map[0xFD] = "BANDAI TAMA5";
    map[0xFE] = "HuC3";
    map[0xFF] = "HuC1+RAM+BATTERY";

    return map[_data[0x147]];
}

size_t Cartridge::getRomSize() { return 32 << _data[0x148]; }

size_t Cartridge::getRamSize() {
    static size_t map[4];
    map[0] = 0;
    map[1] = 2;
    map[2] = 8;
    map[3] = 32;

    return map[_data[0x149]];
}

std::string Cartridge::getDestination() {
    return _data[0x14A] == 0 ? "Japanese" : "Non-japanese";
}

unsigned char Cartridge::getHeaderChecksum() {
    return static_cast<unsigned char>(_data[0x14D]);
}

unsigned char Cartridge::computeHeaderChecksum() {
    unsigned char checksum = 0;
    for (int i = 0x134; i <= 0x14C; ++i) {
        checksum = checksum - _data[i] - 1;
    }
    return checksum;
}