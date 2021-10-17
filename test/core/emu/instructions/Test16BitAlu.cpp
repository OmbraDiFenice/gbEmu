#include <Pch.h>

#include <gtest/gtest.h>
#include "../TestInstructions.h"

class ALU : public TestInstruction {};

#define ADD(opcode, reg)                            \
    TEST_F(ALU, ADD_##opcode##_HL_##reg) {          \
        setNextInstruction(opcode);                 \
        cpu.HL             = Word{0x0F05};          \
        cpu.reg            = Word{0xF106};          \
        Word expectedValue = cpu.HL + cpu.reg;      \
        cpu.setFlags("ZNhc");                       \
        runAndCheck([&expectedValue](Cpu& cpu) {    \
            cpu.PC += 1;                            \
            cpu.HL = expectedValue;                 \
            if (#reg == "HL") {                     \
                cpu.setFlags("znhC");               \
            } else {                                \
                cpu.setFlags("znHC");               \
            }                                       \
        });                                         \
    }                                               \
    TEST_F(ALU, ADD_##opcode##_HL_##reg##_toZero) { \
        setNextInstruction(opcode);                 \
        cpu.HL  = Word{0x8000};                     \
        cpu.reg = Word{0x8000};                     \
        cpu.setFlags("zNHc");                       \
        runAndCheck([](Cpu& cpu) {                  \
            cpu.PC += 1;                            \
            cpu.HL = Word{0x00};                    \
            cpu.setFlags("ZnhC");                   \
        });                                         \
    }

ADD(0x09, BC);
ADD(0x19, DE);
ADD(0x29, HL);
ADD(0x39, SP);
TEST_F(ALU, ADD_SP_n) {
    setNextInstruction(0xE8, Byte{0xFE});
    cpu.SP = Word{0xFF02};
    cpu.setFlags("zNhc");
    runAndCheck([](Cpu& cpu) {
        cpu.PC += 2;
        cpu.SP = Word{0x0000};
        cpu.setFlags("ZnHC");
    });
}

#define INC(opcode, reg)              \
    TEST_F(ALU, INC_##opcode_##reg) { \
        setNextInstruction(opcode);   \
        cpu.reg = Word{0x0FFF};       \
        runAndCheck([](Cpu& cpu) {    \
            cpu.PC += 1;              \
            cpu.reg = Word{0x1000};   \
        });                           \
    }

INC(0x03, BC);
INC(0x13, DE);
INC(0x23, HL);
INC(0x33, SP);

#define DEC(opcode, reg)              \
    TEST_F(ALU, DEC_##opcode_##reg) { \
        setNextInstruction(opcode);   \
        cpu.reg = Word{0x0FFF};       \
        runAndCheck([](Cpu& cpu) {    \
            cpu.PC += 1;              \
            cpu.reg = Word{0x0FFE};   \
        });                           \
    }

DEC(0x0B, BC);
DEC(0x1B, DE);
DEC(0x2B, HL);
DEC(0x3B, SP);
