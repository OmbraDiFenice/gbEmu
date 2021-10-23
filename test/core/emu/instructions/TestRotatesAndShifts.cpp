#include <core/emu/TestInstructions.h>
#include <gtest/gtest.h>

class TestRotatesAndShifts : public TestInstruction {};

#define ROTATE_LEFT(opcode, reg)                 \
    TEST_F(TestRotatesAndShifts, RLC_##opcode) { \
        setNextInstructions({opcode, opcode});   \
        cpu.HL = Word{0xFF10};                   \
                                                 \
        cpu.reg = 0x85;                          \
        cpu.setFlags("ZNHc");                    \
        runAndCheck([](Cpu& cpu) {               \
            cpu.PC += opcode > 0xFF ? 2 : 1;     \
            cpu.reg = 0x0B;                      \
            cpu.setFlags("znhC");                \
        });                                      \
                                                 \
        cpu.reg = 0x00;                          \
        cpu.setFlags("zNHC");                    \
        runAndCheck([](Cpu& cpu) {               \
            cpu.PC += opcode > 0xFF ? 2 : 1;     \
            cpu.reg = 0x00;                      \
            cpu.setFlags("Znhc");                \
        });                                      \
    }

#define ROTATE_LEFT_THROUGH_CARRY(opcode, reg)         \
    TEST_F(TestRotatesAndShifts, RL_##opcode) {        \
        setNextInstructions({opcode, opcode, opcode}); \
        cpu.HL = Word{0xFF10};                         \
                                                       \
        cpu.reg = 0x85;                                \
        cpu.setFlags("ZNHc");                          \
        runAndCheck([](Cpu& cpu) {                     \
            cpu.PC += opcode > 0xFF ? 2 : 1;           \
            cpu.reg = 0x0A;                            \
            cpu.setFlags("znhC");                      \
        });                                            \
                                                       \
        cpu.reg = 0x00;                                \
        cpu.setFlags("zNHC");                          \
        runAndCheck([](Cpu& cpu) {                     \
            cpu.PC += opcode > 0xFF ? 2 : 1;           \
            cpu.reg = 0x01;                            \
            cpu.setFlags("znhc");                      \
        });                                            \
                                                       \
        cpu.reg = 0x00;                                \
        cpu.setFlags("zNHc");                          \
        runAndCheck([](Cpu& cpu) {                     \
            cpu.PC += opcode > 0xFF ? 2 : 1;           \
            cpu.reg = 0x00;                            \
            cpu.setFlags("Znhc");                      \
        });                                            \
    }

#define ROTATE_RIGHT(opcode, reg)                \
    TEST_F(TestRotatesAndShifts, RRC_##opcode) { \
        setNextInstructions({opcode, opcode});   \
        cpu.HL = Word{0xFF10};                   \
                                                 \
        cpu.reg = 0x85;                          \
        cpu.setFlags("ZNHc");                    \
        runAndCheck([](Cpu& cpu) {               \
            cpu.PC += opcode > 0xFF ? 2 : 1;     \
            cpu.reg = 0xC2;                      \
            cpu.setFlags("znhC");                \
        });                                      \
                                                 \
        cpu.reg = 0x00;                          \
        cpu.setFlags("zNHC");                    \
        runAndCheck([](Cpu& cpu) {               \
            cpu.PC += opcode > 0xFF ? 2 : 1;     \
            cpu.reg = 0x00;                      \
            cpu.setFlags("Znhc");                \
        });                                      \
    }

#define ROTATE_RIGHT_THROUGH_CARRY(opcode, reg)        \
    TEST_F(TestRotatesAndShifts, RR_##opcode) {        \
        setNextInstructions({opcode, opcode, opcode}); \
        cpu.HL = Word{0xFF10};                         \
                                                       \
        cpu.reg = 0x85;                                \
        cpu.setFlags("ZNHc");                          \
        runAndCheck([](Cpu& cpu) {                     \
            cpu.PC += opcode > 0xFF ? 2 : 1;           \
            cpu.reg = 0x42;                            \
            cpu.setFlags("znhC");                      \
        });                                            \
                                                       \
        cpu.reg = 0x00;                                \
        cpu.setFlags("zNHC");                          \
        runAndCheck([](Cpu& cpu) {                     \
            cpu.PC += opcode > 0xFF ? 2 : 1;           \
            cpu.reg = 0x80;                            \
            cpu.setFlags("znhc");                      \
        });                                            \
                                                       \
        cpu.reg = 0x00;                                \
        cpu.setFlags("zNHc");                          \
        runAndCheck([](Cpu& cpu) {                     \
            cpu.PC += opcode > 0xFF ? 2 : 1;           \
            cpu.reg = 0x00;                            \
            cpu.setFlags("Znhc");                      \
        });                                            \
    }

ROTATE_LEFT(0x07, A);
ROTATE_LEFT(0xCB07, A);
ROTATE_LEFT(0xCB00, B);
ROTATE_LEFT(0xCB01, C);
ROTATE_LEFT(0xCB02, D);
ROTATE_LEFT(0xCB03, E);
ROTATE_LEFT(0xCB04, H);
ROTATE_LEFT(0xCB05, L);
ROTATE_LEFT(0xCB06, memory[cpu.HL]);

ROTATE_LEFT_THROUGH_CARRY(0x17, A);
ROTATE_LEFT_THROUGH_CARRY(0xCB17, A);
ROTATE_LEFT_THROUGH_CARRY(0xCB10, B);
ROTATE_LEFT_THROUGH_CARRY(0xCB11, C);
ROTATE_LEFT_THROUGH_CARRY(0xCB12, D);
ROTATE_LEFT_THROUGH_CARRY(0xCB13, E);
ROTATE_LEFT_THROUGH_CARRY(0xCB14, H);
ROTATE_LEFT_THROUGH_CARRY(0xCB15, L);
ROTATE_LEFT_THROUGH_CARRY(0xCB16, memory[cpu.HL]);

ROTATE_RIGHT(0x0F, A);
ROTATE_RIGHT(0xCB0F, A);
ROTATE_RIGHT(0xCB08, B);
ROTATE_RIGHT(0xCB09, C);
ROTATE_RIGHT(0xCB0A, D);
ROTATE_RIGHT(0xCB0B, E);
ROTATE_RIGHT(0xCB0C, H);
ROTATE_RIGHT(0xCB0D, L);
ROTATE_RIGHT(0xCB0E, memory[cpu.HL]);

ROTATE_RIGHT_THROUGH_CARRY(0x1F, A);
ROTATE_RIGHT_THROUGH_CARRY(0xCB1F, A);
ROTATE_RIGHT_THROUGH_CARRY(0xCB18, B);
ROTATE_RIGHT_THROUGH_CARRY(0xCB19, C);
ROTATE_RIGHT_THROUGH_CARRY(0xCB1A, D);
ROTATE_RIGHT_THROUGH_CARRY(0xCB1B, E);
ROTATE_RIGHT_THROUGH_CARRY(0xCB1C, H);
ROTATE_RIGHT_THROUGH_CARRY(0xCB1D, L);
ROTATE_RIGHT_THROUGH_CARRY(0xCB1E, memory[cpu.HL]);
