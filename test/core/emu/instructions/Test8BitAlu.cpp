#include <Pch.h>

#include <gtest/gtest.h>
#include "../TestInstructions.h"

class ALU : public TestInstruction {};

#define ADD(opcode, reg)                           \
    TEST_F(ALU, ADD_##opcode##_A_##reg) {          \
        setNextInstruction(opcode);                \
        cpu.A              = Byte{0x05};           \
        cpu.reg            = Byte{0x06};           \
        Byte expectedValue = cpu.A + cpu.reg;      \
        runAndCheck([&expectedValue](Cpu& cpu) {   \
            cpu.PC += 1;                           \
            cpu.A = expectedValue;                 \
            cpu.setFlags("znhc");                  \
        });                                        \
    }                                              \
    TEST_F(ALU, ADD_##opcode##_A_##reg##_toZero) { \
        setNextInstruction(opcode);                \
        cpu.A   = Byte{0x80};                      \
        cpu.reg = Byte{0x80};                      \
        runAndCheck([](Cpu& cpu) {                 \
            cpu.PC += 1;                           \
            cpu.A = Byte{0x00};                    \
            cpu.setFlags("ZnhC");                  \
        });                                        \
    }

ADD(0x87, A);
ADD(0x80, B);
ADD(0x81, C);
ADD(0x82, D);
ADD(0x83, E);
ADD(0x84, H);
ADD(0x85, L);
TEST_F(ALU, ADD_A__HL__) {
    setNextInstruction(0x86);
    cpu.A              = Byte{0x05};
    cpu.HL             = Word{0xFF10};
    cpu.memory[0xFF10] = Byte{0x67};
    runAndCheck([](Cpu& cpu) {
        cpu.PC += 1;
        cpu.A = Byte{0x05 + 0x67};
    });
}
TEST_F(ALU, ADD_A_IMM) {
    setNextInstruction(0xC6, Byte{0x67});
    cpu.A = Byte{0x05};
    runAndCheck([](Cpu& cpu) {
        cpu.PC += 2;
        cpu.A = Byte{0x05 + 0x67};
    });
}
