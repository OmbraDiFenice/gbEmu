#include <Pch.h>

#include <core/emu/Cpu.h>

#define LD_REG_IMM(opcode, reg)                \
    CPU_INSTRUCTION(opcode) {                  \
        Byte immediate = cpu.memory[cpu.PC++]; \
        cpu.reg        = immediate;            \
    }

#define LD_REG16_IMM(opcode, reg)                               \
    CPU_INSTRUCTION(opcode) {                                   \
        Byte immediateMsb = cpu.memory[cpu.PC++];               \
        Byte immediateLsb = cpu.memory[cpu.PC++];               \
        Word immediate    = (immediateMsb << 8) | immediateLsb; \
        cpu.reg           = immediate;                          \
    }

LD_REG_IMM(0x3E, A);  // LD A, n
LD_REG_IMM(0x06, B);  // LD B, n
LD_REG_IMM(0x0E, C);  // LD C, n
LD_REG_IMM(0x16, D);  // LD D, n
LD_REG_IMM(0x1E, E);  // LD E, n
LD_REG_IMM(0x26, H);  // LD H, n
LD_REG_IMM(0x2E, L);  // LD L, n

#define LD_REG_REG(opcode, dst, src) \
    CPU_INSTRUCTION(opcode) { cpu.dst = cpu.src; }

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

CPU_INSTRUCTION(0xE0) {  // LD (n), A
    Byte immediate   = cpu.memory[cpu.PC++];
    size_t addr      = 0xFF00 + immediate;
    cpu.memory[addr] = cpu.A;
}

CPU_INSTRUCTION(0xF0) {  // LD A, (n)
    Byte immediate = cpu.memory[cpu.PC++];
    size_t addr    = 0xFF00 + immediate;
    cpu.A          = cpu.memory[addr];
}

LD_REG16_IMM(0x01, BC);  // LD BC, nn
LD_REG16_IMM(0x11, DE);  // LD DE, nn
LD_REG16_IMM(0x21, HL);  // LD HL, nn
LD_REG16_IMM(0x31, SP);  // LD SP, nn

LD_REG_REG(0xF9, SP, HL);  // LD SP, HL

CPU_INSTRUCTION(0xF8) {  // LD HL, SP+n
    Word immediate = cpu.memory[cpu.PC++];
    cpu.HL         = cpu.sum(cpu.SP, immediate);
    cpu.setFlag(Cpu::Flag::Z, false);
    cpu.setFlag(Cpu::Flag::N, false);
}

CPU_INSTRUCTION(0x08) {  // LD (nn), SP
    Word immediateMsb    = cpu.memory[cpu.PC++];
    Word immediateLsb    = cpu.memory[cpu.PC++];
    Word addr            = (immediateMsb << 8) | immediateLsb;
    cpu.memory[addr]     = cpu.SP.msb;
    cpu.memory[addr + 1] = cpu.SP.lsb;
}

#define PUSH(opcode, reg)                   \
    CPU_INSTRUCTION(opcode) {               \
        cpu.memory[--cpu.SP] = cpu.reg.msb; \
        cpu.memory[--cpu.SP] = cpu.reg.lsb; \
    }

PUSH(0xF5, AF);  // PUSH AF
PUSH(0xC5, BC);  // PUSH BC
PUSH(0xD5, DE);  // PUSH DE
PUSH(0xE5, HL);  // PUSH HL

#define POP(opcode, reg)                    \
    CPU_INSTRUCTION(opcode) {               \
        cpu.reg.lsb = cpu.memory[cpu.SP++]; \
        cpu.reg.msb = cpu.memory[cpu.SP++]; \
    }

POP(0xF1, AF);  // POP AF
POP(0xC1, BC);  // POP BC
POP(0xD1, DE);  // POP DE
POP(0xE1, HL);  // POP HL

#define ADD(opcode, reg)                                \
    CPU_INSTRUCTION(opcode) {                           \
        cpu.A = cpu.sum(cpu.A, cpu.reg);                \
        cpu.setFlag(Cpu::Flag::Z, cpu.A == Byte{0x00}); \
        cpu.setFlag(Cpu::Flag::N, false);               \
    }

ADD(0x87, A);                 // ADD A, A
ADD(0x80, B);                 // ADD A, B
ADD(0x81, C);                 // ADD A, C
ADD(0x82, D);                 // ADD A, D
ADD(0x83, E);                 // ADD A, E
ADD(0x84, H);                 // ADD A, H
ADD(0x85, L);                 // ADD A, L
ADD(0x86, memory[cpu.HL]);    // ADD A, (HL)
ADD(0xC6, memory[cpu.PC++]);  // ADD A, #

#define ADC(opcode, reg)                                \
    CPU_INSTRUCTION(opcode) {                           \
        cpu.A = cpu.sum(cpu.A, cpu.reg, true);          \
        cpu.setFlag(Cpu::Flag::Z, cpu.A == Byte{0x00}); \
        cpu.setFlag(Cpu::Flag::N, false);               \
    }

ADC(0x8F, A);                 // ADC A, A
ADC(0x88, B);                 // ADC A, B
ADC(0x89, C);                 // ADC A, C
ADC(0x8A, D);                 // ADC A, D
ADC(0x8B, E);                 // ADC A, E
ADC(0x8C, H);                 // ADC A, H
ADC(0x8D, L);                 // ADC A, L
ADC(0x8E, memory[cpu.HL]);    // ADC A, (HL)
ADC(0xCE, memory[cpu.PC++]);  // ADC A, #

#define SUB(opcode, reg)                                \
    CPU_INSTRUCTION(opcode) {                           \
        cpu.A = cpu.sub(cpu.A, cpu.reg);                \
        cpu.setFlag(Cpu::Flag::Z, cpu.A == Byte{0x00}); \
        cpu.setFlag(Cpu::Flag::N, true);                \
    }

SUB(0x97, A);                 // SUB A, A
SUB(0x90, B);                 // SUB A, B
SUB(0x91, C);                 // SUB A, C
SUB(0x92, D);                 // SUB A, D
SUB(0x93, E);                 // SUB A, E
SUB(0x94, H);                 // SUB A, H
SUB(0x95, L);                 // SUB A, L
SUB(0x96, memory[cpu.HL]);    // SUB A, (HL)
SUB(0xD6, memory[cpu.PC++]);  // SUB A, #

#define SBC(opcode, reg)                                \
    CPU_INSTRUCTION(opcode) {                           \
        cpu.A = cpu.sub(cpu.A, cpu.reg, true);          \
        cpu.setFlag(Cpu::Flag::Z, cpu.A == Byte{0x00}); \
        cpu.setFlag(Cpu::Flag::N, true);                \
    }

SBC(0x9F, A);               // SBC A, A
SBC(0x98, B);               // SBC A, B
SBC(0x99, C);               // SBC A, C
SBC(0x9A, D);               // SBC A, D
SBC(0x9B, E);               // SBC A, E
SBC(0x9C, H);               // SBC A, H
SBC(0x9D, L);               // SBC A, L
SBC(0x9E, memory[cpu.HL]);  // SBC A, (HL)

#define AND(opcode, reg)                                \
    CPU_INSTRUCTION(opcode) {                           \
        cpu.A = cpu.A & cpu.reg;                        \
        cpu.setFlag(Cpu::Flag::Z, cpu.A == Byte{0x00}); \
        cpu.setFlags("nHc");                            \
    }

AND(0xA7, A);                 // AND A, A
AND(0xA0, B);                 // AND A, B
AND(0xA1, C);                 // AND A, C
AND(0xA2, D);                 // AND A, D
AND(0xA3, E);                 // AND A, E
AND(0xA4, H);                 // AND A, H
AND(0xA5, L);                 // AND A, L
AND(0xA6, memory[cpu.HL]);    // AND A, (HL)
AND(0xE6, memory[cpu.PC++]);  // AND A, #

#define OR(opcode, reg)                                 \
    CPU_INSTRUCTION(opcode) {                           \
        cpu.A = cpu.A | cpu.reg;                        \
        cpu.setFlag(Cpu::Flag::Z, cpu.A == Byte{0x00}); \
        cpu.setFlags("nhc");                            \
    }

OR(0xB7, A);                 // OR A, A
OR(0xB0, B);                 // OR A, B
OR(0xB1, C);                 // OR A, C
OR(0xB2, D);                 // OR A, D
OR(0xB3, E);                 // OR A, E
OR(0xB4, H);                 // OR A, H
OR(0xB5, L);                 // OR A, L
OR(0xB6, memory[cpu.HL]);    // OR A, (HL)
OR(0xF6, memory[cpu.PC++]);  // OR A, #

#define XOR(opcode, reg)                                \
    CPU_INSTRUCTION(opcode) {                           \
        cpu.A = cpu.A ^ cpu.reg;                        \
        cpu.setFlag(Cpu::Flag::Z, cpu.A == Byte{0x00}); \
        cpu.setFlags("nhc");                            \
    }

XOR(0xAF, A);                 // XOR A, A
XOR(0xA8, B);                 // XOR A, B
XOR(0xA9, C);                 // XOR A, C
XOR(0xAA, D);                 // XOR A, D
XOR(0xAB, E);                 // XOR A, E
XOR(0xAC, H);                 // XOR A, H
XOR(0xAD, L);                 // XOR A, L
XOR(0xAE, memory[cpu.HL]);    // XOR A, (HL)
XOR(0xEE, memory[cpu.PC++]);  // XOR A, #

#define CP(opcode, reg)                            \
    CPU_INSTRUCTION(opcode) {                      \
        Byte result = cpu.sub(cpu.A, cpu.reg);     \
        cpu.setFlag(Cpu::Flag::Z, result == 0x00); \
        cpu.setFlag(Cpu::Flag::N, true);           \
    }

CP(0xBF, A);                 // CP A, A
CP(0xB8, B);                 // CP A, B
CP(0xB9, C);                 // CP A, C
CP(0xBA, D);                 // CP A, D
CP(0xBB, E);                 // CP A, E
CP(0xBC, H);                 // CP A, H
CP(0xBD, L);                 // CP A, L
CP(0xBE, memory[cpu.HL]);    // CP A, (HL)
CP(0xFE, memory[cpu.PC++]);  // CP A, #

#define INC(opcode, reg)                                  \
    CPU_INSTRUCTION(opcode) {                             \
        bool initialC = cpu.getFlag(Cpu::Flag::C);        \
        cpu.reg       = cpu.sum(cpu.reg, 0x01);           \
        cpu.setFlag(Cpu::Flag::Z, cpu.reg == Byte{0x00}); \
        cpu.setFlag(Cpu::Flag::N, false);                 \
        cpu.setFlag(Cpu::Flag::C, initialC);              \
    }

INC(0x3C, A);               // INC A
INC(0x04, B);               // INC B
INC(0x0C, C);               // INC C
INC(0x14, D);               // INC D
INC(0x1C, E);               // INC E
INC(0x24, H);               // INC H
INC(0x2C, L);               // INC L
INC(0x34, memory[cpu.HL]);  // INC (HL)

#define DEC(opcode, reg)                                  \
    CPU_INSTRUCTION(opcode) {                             \
        bool initialC = cpu.getFlag(Cpu::Flag::C);        \
        cpu.reg       = cpu.sub(cpu.reg, 0x01);           \
        cpu.setFlag(Cpu::Flag::Z, cpu.reg == Byte{0x00}); \
        cpu.setFlag(Cpu::Flag::N, true);                  \
        cpu.setFlag(Cpu::Flag::C, initialC);              \
    }

DEC(0x3D, A);               // DEC A
DEC(0x05, B);               // DEC B
DEC(0x0D, C);               // DEC C
DEC(0x15, D);               // DEC D
DEC(0x1D, E);               // DEC E
DEC(0x25, H);               // DEC H
DEC(0x2D, L);               // DEC L
DEC(0x35, memory[cpu.HL]);  // DEC (HL)

#define ADD16(opcode, reg)                                       \
    CPU_INSTRUCTION(opcode) {                                    \
        cpu.HL = cpu.sum(cpu.HL, cpu.reg);                       \
        cpu.setFlag(Cpu::Flag::Z, Word{cpu.HL} == Word{0x0000}); \
        cpu.setFlag(Cpu::Flag::N, false);                        \
    }

ADD16(0x09, BC);         // ADD HL, BC
ADD16(0x19, DE);         // ADD HL, DE
ADD16(0x29, HL);         // ADD HL, HL
ADD16(0x39, SP);         // ADD HL, SP
CPU_INSTRUCTION(0xE8) {  // ADD SP, n
    cpu.SP = cpu.sum(cpu.SP, Word{cpu.memory[cpu.PC++]});
    cpu.setFlag(Cpu::Flag::Z, Word{cpu.SP} == Word{0x0000});
    cpu.setFlag(Cpu::Flag::N, false);
}
