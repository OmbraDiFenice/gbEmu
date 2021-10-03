#include <Pch.h>

#include <gtest/gtest.h>
#include "../TestInstructions.h"

class LD : public TestInstruction {};

#define LD_REG16_REG16(opcode, dst, src)      \
    TEST_F(LD, LD_##opcode##_##dst##_##src) { \
        setNextInstruction(opcode);           \
        cpu.dst = Word{0x00};                 \
        cpu.src = Word{0x6789};               \
        runAndCheck([](Cpu& cpu) {            \
            cpu.PC += 1;                      \
            cpu.dst = Word{0x6789};           \
        });                                   \
    }

LD_REG16_REG16(0xF9, SP, HL);

TEST_F(LD, LDHL_SP_IMM) {
    setNextInstruction(0xF8, Byte{0x14});
    cpu.HL = Word{0x00};
    cpu.SP = Word{0xFFF0};
    cpu.setFlags("ZNhc");
    runAndCheck([](Cpu& cpu) {
        cpu.PC += 2;
        cpu.setFlags("znHC");
        cpu.HL = Word{0x0004};
    });
}

TEST_F(LD, LD__IMMaddr__SP) {
    setNextInstruction(0x08, Word{0x1122});
    cpu.SP             = Word{0x6789};
    cpu.memory[0x1122] = Byte{0x00};
    cpu.memory[0x1123] = Byte{0x00};
    runAndCheck([](Cpu& cpu) {
        cpu.PC += 3;
        cpu.memory[0x1122] = Byte{0x67};
        cpu.memory[0x1123] = Byte{0x89};
    });
}

#define PUSH(opcode, reg)                    \
    TEST_F(LD, PUSH_##reg) {                 \
        setNextInstruction(opcode);          \
        cpu.SP             = Word{0x1000};   \
        cpu.reg            = Word{0x6789};   \
        cpu.memory[0x0FFF] = Byte{0x00};     \
        cpu.memory[0x0FFE] = Byte{0x00};     \
        runAndCheck([](Cpu& cpu) {           \
            cpu.PC += 1;                     \
            cpu.SP -= 2;                     \
            cpu.memory[0x0FFF] = Byte{0x67}; \
            cpu.memory[0x0FFE] = Byte{0x89}; \
        });                                  \
    }

PUSH(0xF5, AF);
PUSH(0xC5, BC);
PUSH(0xD5, DE);
PUSH(0xE5, HL);

#define POP(opcode, reg)                   \
    TEST_F(LD, POP_##reg) {                \
        setNextInstruction(opcode);        \
        cpu.SP             = Word{0x1000}; \
        cpu.memory[0x1000] = Byte{0x89};   \
        cpu.memory[0x1001] = Byte{0x67};   \
        cpu.reg            = Word{0x0000}; \
        runAndCheck([](Cpu& cpu) {         \
            cpu.PC += 1;                   \
            cpu.SP += 2;                   \
            cpu.reg = Word{0x6789};        \
        });                                \
    }

POP(0xF1, AF);
POP(0xC1, BC);
POP(0xD1, DE);
POP(0xE1, HL);
