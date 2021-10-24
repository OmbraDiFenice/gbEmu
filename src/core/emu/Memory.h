#pragma once

using Byte  = uint8_t;
using Word  = uint16_t;

class Memory {
   public:
    static constexpr size_t kMemSize = 0x10000;

    Memory() = default;
    Memory(const Memory& iOther);
    void zero();

    Byte& operator[](Word addr);

   protected:
    Byte _data[kMemSize];
};
