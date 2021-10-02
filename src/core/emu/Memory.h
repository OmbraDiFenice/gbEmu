#pragma once

using Byte  = uint8_t;
using Word  = uint16_t;
using Dword = uint32_t;

#define LEN_BYTE 8u
#define LEN_WORD 16u
#define LEN_DWORD 32u

class Memory {
   public:
    explicit Memory(size_t bytes);
    virtual ~Memory();

    void write(size_t addr, Byte b);
    void write2(size_t addr, Word w);
    void write4(size_t addr, Dword dw);

    Byte read(size_t addr);
    Word read2(size_t addr);
    Dword read4(size_t addr);

   protected:
    inline bool doesDataFit(size_t addr, size_t bytes) {
        return addr + bytes <= size;
    }

   public:
    const size_t size;

   protected:
    Byte* _data;
};
