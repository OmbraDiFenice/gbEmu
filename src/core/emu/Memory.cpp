#include <Pch.h>

#include <core/emu/Memory.h>

Memory::Memory(const Memory& iOther) {
    memcpy(_data, iOther._data, sizeof(Byte) * Memory::kMemSize);
}

void Memory::zero() { std::memset(_data, 0, Memory::kMemSize); }

Byte& Memory::operator[](Word addr) { return _data[addr]; }
