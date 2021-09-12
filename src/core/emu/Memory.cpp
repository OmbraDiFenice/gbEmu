#include <Pch.h>

#include <core/emu/Memory.h>

Memory::Memory(size_t bytes) : size(bytes) { _data = new Byte[bytes]; }

Memory::~Memory() { delete[] _data; }

Byte Memory::read(size_t addr) {
    if (addr >= size) {
        LOG_WARN("reading at address " << addr << " which exceedes memory size "
                                       << size);
        return 0;
    }
    return _data[addr];
}

Word Memory::read2(size_t addr) {
    return (read(addr) << LEN_BYTE) | read(addr + 1);
}

Dword Memory::read4(size_t addr) {
    return (read2(addr) << LEN_WORD) | read2(addr + 2);
}

void Memory::write(size_t addr, Byte b) {
    if (!doesDataFit(addr, 1)) {
        LOG_WARN("writing at address " << addr << " which exceedes memory size "
                                       << size);
        return;
    }
    _data[addr] = b;
}

void Memory::write2(size_t addr, Word w) {
    if (!doesDataFit(addr, 2)) {
        LOG_WARN("writing at address " << addr << " which exceedes memory size "
                                       << size);
        return;
    }
    write(addr, (w & 0xFF00) >> LEN_BYTE);
    write(addr + 1, w & 0xFF);
}

void Memory::write4(size_t addr, Dword d) {
    if (!doesDataFit(addr, 4)) {
        LOG_WARN("writing at address " << addr << " which exceedes memory size "
                                       << size);
        return;
    }
    write2(addr, (d & 0xFFFF0000) >> LEN_WORD);
    write2(addr + 2, d & 0xFFFF);
}