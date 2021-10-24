#pragma once

#include <core/emu/Memory.h>
#include <string>

class Cartridge {
   public:
    ~Cartridge();

    bool load(const std::string& filename);

    std::string getTitle();
    std::string getCartType();
    size_t getRomSize();
    size_t getRamSize();
    std::string getDestination();
    Byte getHeaderChecksum();
    Byte computeHeaderChecksum();

   private:
    Byte* _data = nullptr;
};
