#include <core/emu/TestInstructions.h>
#include <gtest/gtest.h>

class TestBitOperations : public TestInstruction {};

#define BIT(opcode, reg)                             \
    TEST_F(TestBitOperations, BIT_IMM_##opcode) {    \
        setNextInstructions({{opcode, Byte{0x01}},   \
                             {opcode, Byte{0x00}},   \
                             {opcode, Byte{0x07}}}); \
        cpu.HL  = Word{0xFF10};                      \
        cpu.reg = Byte{0x03};                        \
                                                     \
        cpu.setFlags("ZNh");                         \
        runAndCheck([](Cpu& cpu) {                   \
            cpu.PC += 3;                             \
            cpu.setFlags("znH");                     \
        });                                          \
                                                     \
        cpu.setFlags("ZNh");                         \
        runAndCheck([](Cpu& cpu) {                   \
            cpu.PC += 3;                             \
            cpu.setFlags("znH");                     \
        });                                          \
                                                     \
        cpu.setFlags("zNh");                         \
        runAndCheck([](Cpu& cpu) {                   \
            cpu.PC += 3;                             \
            cpu.setFlags("ZnH");                     \
        });                                          \
    }

BIT(0xCB47, A);
BIT(0xCB40, B);
BIT(0xCB41, C);
BIT(0xCB42, D);
BIT(0xCB43, E);
BIT(0xCB44, H);
BIT(0xCB45, L);
BIT(0xCB46, memory[cpu.HL]);

#define SET(opcode, reg)                          \
    TEST_F(TestBitOperations, SET_IMM_##opcode) { \
        setNextInstruction(opcode, Byte{0x06});   \
        cpu.HL  = Word{0xFF10};                   \
        cpu.reg = Byte{0x00};                     \
        runAndCheck([](Cpu& cpu) {                \
            cpu.PC += 3;                          \
            cpu.reg = Byte{0x40};                 \
        });                                       \
    }

SET(0xCBC7, A);
SET(0xCBC0, B);
SET(0xCBC1, C);
SET(0xCBC2, D);
SET(0xCBC3, E);
SET(0xCBC4, H);
SET(0xCBC5, L);
SET(0xCBC6, memory[cpu.HL]);

#define RES(opcode, reg)                          \
    TEST_F(TestBitOperations, RES_IMM_##opcode) { \
        setNextInstruction(opcode, Byte{0x04});   \
        cpu.HL  = Word{0xFF10};                   \
        cpu.reg = Byte{0x35};                     \
        runAndCheck([](Cpu& cpu) {                \
            cpu.PC += 3;                          \
            cpu.reg = Byte{0x25};                 \
        });                                       \
    }

RES(0xCB87, A);
RES(0xCB80, B);
RES(0xCB81, C);
RES(0xCB82, D);
RES(0xCB83, E);
RES(0xCB84, H);
RES(0xCB85, L);
RES(0xCB86, memory[cpu.HL]);
