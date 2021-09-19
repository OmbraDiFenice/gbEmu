#include <Pch.h>

#include <core/emu/Cpu.h>

CPU_INSTRUCTION(0x06) {  // LD B, n
    Byte immediate = cpu.memory[cpu.PC++];
    cpu.B          = immediate;
}
CPU_INSTRUCTION(0x0E) {  // LD C, n
    Byte immediate = cpu.memory[cpu.PC++];
    cpu.C          = immediate;
}
CPU_INSTRUCTION(0x16) {  // LD D, n
    Byte immediate = cpu.memory[cpu.PC++];
    cpu.D          = immediate;
}
CPU_INSTRUCTION(0x1E) {  // LD E, n
    Byte immediate = cpu.memory[cpu.PC++];
    cpu.E          = immediate;
}
CPU_INSTRUCTION(0x26) {  // LD H, n
    Byte immediate = cpu.memory[cpu.PC++];
    cpu.H          = immediate;
}
CPU_INSTRUCTION(0x2E) {  // LD L, n
    Byte immediate = cpu.memory[cpu.PC++];
    cpu.L          = immediate;
}
