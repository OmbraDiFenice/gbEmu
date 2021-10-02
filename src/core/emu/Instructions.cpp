#include <Pch.h>

#include <core/emu/Cpu.h>

#define LD_REG_IMM(opcode, reg)                \
    CPU_INSTRUCTION(opcode) {                  \
        Byte immediate = cpu.memory[cpu.PC++]; \
        cpu.reg        = immediate;            \
    }

LD_REG_IMM(0x3E, A);  // LD A, n
LD_REG_IMM(0x06, B);  // LD B, n
LD_REG_IMM(0x0E, C);  // LD C, n
LD_REG_IMM(0x16, D);  // LD D, n
LD_REG_IMM(0x1E, E);  // LD E, n
LD_REG_IMM(0x26, H);  // LD H, n
LD_REG_IMM(0x2E, L);  // LD L, n

#define LD_REG_REG(opcode, dst, src) \
    CPU_INSTRUCTION(opcode) {        \
        Byte value = cpu.src;        \
        cpu.dst    = value;          \
    }

#define LD_REG_REGaddr(opcode, dst, regaddr)  \
    CPU_INSTRUCTION(opcode) {                 \
        Byte value = cpu.memory[cpu.regaddr]; \
        cpu.dst    = value;                   \
    }

#define LD_REGaddr_REG(opcode, regaddr, src) \
    CPU_INSTRUCTION(opcode) {                \
        Byte value              = cpu.src;   \
        cpu.memory[cpu.regaddr] = value;     \
    }

#define LD_REGaddr_IMM(opcode, regaddr)                 \
    CPU_INSTRUCTION(opcode) {                           \
        Byte value              = cpu.memory[cpu.PC++]; \
        cpu.memory[cpu.regaddr] = value;                \
    }

#define LD_REG_IMMaddr(opcode, reg)                                      \
    CPU_INSTRUCTION(opcode) {                                            \
        Word addr  = cpu.memory[cpu.PC++] | (cpu.memory[cpu.PC++] << 8); \
        Byte value = cpu.memory[addr];                                   \
        cpu.reg    = value;                                              \
    }

#define LD_IMMaddr_REG(opcode, reg)                                            \
    CPU_INSTRUCTION(opcode) {                                                  \
        Word addr        = cpu.memory[cpu.PC++] | (cpu.memory[cpu.PC++] << 8); \
        cpu.memory[addr] = cpu.reg;                                            \
    }

LD_REG_REG(0x7F, A, A);       // LD A, A
LD_REG_REG(0x78, A, B);       // LD A, B
LD_REG_REG(0x79, A, C);       // LD A, C
LD_REG_REG(0x7A, A, D);       // LD A, D
LD_REG_REG(0x7B, A, E);       // LD A, E
LD_REG_REG(0x7C, A, H);       // LD A, H
LD_REG_REG(0x7D, A, L);       // LD A, L
LD_REG_REGaddr(0x7E, A, HL);  // LD A, (HL)

LD_REG_REG(0x40, B, B);       // LD B, B
LD_REG_REG(0x41, B, C);       // LD B, C
LD_REG_REG(0x42, B, D);       // LD B, D
LD_REG_REG(0x43, B, E);       // LD B, E
LD_REG_REG(0x44, B, H);       // LD B, H
LD_REG_REG(0x45, B, L);       // LD B, L
LD_REG_REGaddr(0x46, B, HL);  // LD B, (HL)

LD_REG_REG(0x48, C, B);       // LD C, B
LD_REG_REG(0x49, C, C);       // LD C, C
LD_REG_REG(0x4A, C, D);       // LD C, D
LD_REG_REG(0x4B, C, E);       // LD C, E
LD_REG_REG(0x4C, C, H);       // LD C, H
LD_REG_REG(0x4D, C, L);       // LD C, L
LD_REG_REGaddr(0x4E, C, HL);  // LD C, (HL)

LD_REG_REG(0x50, D, B);       // LD D, B
LD_REG_REG(0x51, D, C);       // LD D, C
LD_REG_REG(0x52, D, D);       // LD D, D
LD_REG_REG(0x53, D, E);       // LD D, E
LD_REG_REG(0x54, D, H);       // LD D, H
LD_REG_REG(0x55, D, L);       // LD D, L
LD_REG_REGaddr(0x56, D, HL);  // LD D, (HL)

LD_REG_REG(0x58, E, B);       // LD E, B
LD_REG_REG(0x59, E, C);       // LD E, C
LD_REG_REG(0x5A, E, D);       // LD E, D
LD_REG_REG(0x5B, E, E);       // LD E, E
LD_REG_REG(0x5C, E, H);       // LD E, H
LD_REG_REG(0x5D, E, L);       // LD E, L
LD_REG_REGaddr(0x5E, E, HL);  // LD E, (HL)

LD_REG_REG(0x60, H, B);       // LD H, B
LD_REG_REG(0x61, H, C);       // LD H, C
LD_REG_REG(0x62, H, D);       // LD H, D
LD_REG_REG(0x63, H, E);       // LD H, E
LD_REG_REG(0x64, H, H);       // LD H, H
LD_REG_REG(0x65, H, L);       // LD H, L
LD_REG_REGaddr(0x66, H, HL);  // LD H, (HL)

LD_REG_REG(0x68, L, B);       // LD L, B
LD_REG_REG(0x69, L, C);       // LD L, C
LD_REG_REG(0x6A, L, D);       // LD L, D
LD_REG_REG(0x6B, L, E);       // LD L, E
LD_REG_REG(0x6C, L, H);       // LD L, H
LD_REG_REG(0x6D, L, L);       // LD L, L
LD_REG_REGaddr(0x6E, L, HL);  // LD L, (HL)

LD_REGaddr_REG(0x70, HL, B);  // LD HL, B
LD_REGaddr_REG(0x71, HL, C);  // LD HL, C
LD_REGaddr_REG(0x72, HL, D);  // LD HL, D
LD_REGaddr_REG(0x73, HL, E);  // LD HL, E
LD_REGaddr_REG(0x74, HL, H);  // LD HL, H
LD_REGaddr_REG(0x75, HL, L);  // LD HL, L
LD_REGaddr_IMM(0x36, HL);     // LD (HL), n

LD_REG_REGaddr(0x0A, A, BC);  // LD A, (BC)
LD_REG_REGaddr(0x1A, A, DE);  // LD A, (DE)

LD_REG_IMMaddr(0xFA, A);  // LD A, (nn)

LD_REG_REG(0x47, B, A);  // LD B, A
LD_REG_REG(0x4F, C, A);  // LD C, A
LD_REG_REG(0x57, D, A);  // LD D, A
LD_REG_REG(0x5F, E, A);  // LD E, A
LD_REG_REG(0x67, H, A);  // LD H, A
LD_REG_REG(0x6F, L, A);  // LD L, A

LD_REGaddr_REG(0x02, BC, A);  // LD (BC), A
LD_REGaddr_REG(0x12, DE, A);  // LD (DE), A
LD_REGaddr_REG(0x77, HL, A);  // LD (HL), A

LD_IMMaddr_REG(0xEA, A);  // LD (nn), A

CPU_INSTRUCTION(0xF2) {  // LD A, (C)
    Byte value = cpu.memory[0xFF00 + cpu.C];
    cpu.A      = value;
}

CPU_INSTRUCTION(0xE2) {  // LD (C), A
    size_t addr      = 0x0FF00 + cpu.C;
    cpu.memory[addr] = cpu.A;
}

CPU_INSTRUCTION(0x3A) {  // LD A, (HLD)
    Word addr = cpu.HL;
    cpu.A     = cpu.memory[addr];
    --cpu.HL;
}

CPU_INSTRUCTION(0x32) {  // LD (HLD), A
    size_t addr      = cpu.HL;
    cpu.memory[addr] = cpu.A;
    --cpu.HL;
}

CPU_INSTRUCTION(0x2A) {  // LD A, (HLI)
    Word addr = cpu.HL;
    cpu.A     = cpu.memory[addr];
    ++cpu.HL;
}

CPU_INSTRUCTION(0x22) {  // LD (HLI), A
    size_t addr      = cpu.HL;
    cpu.memory[addr] = cpu.A;
    ++cpu.HL;
}
