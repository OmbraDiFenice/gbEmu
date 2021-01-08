#pragma once

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
    unsigned char getHeaderChecksum();
    unsigned char computeHeaderChecksum();

   private:
    char* _data = nullptr;
};
