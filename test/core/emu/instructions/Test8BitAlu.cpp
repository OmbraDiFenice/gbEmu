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
        cpu.setFlag(Cpu::Flag::Z, false);          \
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
    cpu.A              = Byte{0x09};
    cpu.HL             = Word{0xFF10};
    cpu.memory[0xFF10] = Byte{0x67};
    runAndCheck([](Cpu& cpu) {
        cpu.PC += 1;
        cpu.A = Byte{0x09 + 0x67};
        cpu.setFlags("znHc");
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

#define ADC(opcode, reg)                           \
    TEST_F(ALU, ADC_##opcode##_A_##reg) {          \
        setNextInstruction(opcode);                \
        cpu.A = Byte{0x05};                        \
        cpu.setFlag(Cpu::Flag::C, true);           \
        cpu.reg            = Byte{0x06};           \
        Byte expectedValue = cpu.A + cpu.reg + 1;  \
        runAndCheck([&expectedValue](Cpu& cpu) {   \
            cpu.PC += 1;                           \
            cpu.A = expectedValue;                 \
            cpu.setFlags("znhc");                  \
        });                                        \
    }                                              \
    TEST_F(ALU, ADC_##opcode##_A_##reg##_toZero) { \
        setNextInstruction(opcode);                \
        cpu.A = Byte{0x80};                        \
        cpu.setFlag(Cpu::Flag::Z, false);          \
        cpu.setFlag(Cpu::Flag::C, false);          \
        cpu.reg = Byte{0x80};                      \
        runAndCheck([](Cpu& cpu) {                 \
            cpu.PC += 1;                           \
            cpu.A = Byte{0x00};                    \
            cpu.setFlags("ZnhC");                  \
        });                                        \
    }

ADC(0x8F, A);
ADC(0x88, B);
ADC(0x89, C);
ADC(0x8A, D);
ADC(0x8B, E);
ADC(0x8C, H);
ADC(0x8D, L);
TEST_F(ALU, ADC_A__HL__) {
    setNextInstruction(0x8E);
    cpu.A              = Byte{0x08};
    cpu.HL             = Word{0xFF10};
    cpu.memory[0xFF10] = Byte{0x67};
    cpu.setFlag(Cpu::Flag::C, true);
    runAndCheck([](Cpu& cpu) {
        cpu.PC += 1;
        cpu.A = Byte{0x08 + 0x67 + 0x01};
        cpu.setFlags("znHc");
    });
}
TEST_F(ALU, ADC_A_IMM) {
    setNextInstruction(0xCE, Byte{0x0B});
    cpu.A = Byte{0xF4};
    cpu.setFlag(Cpu::Flag::C, true);
    runAndCheck([](Cpu& cpu) {
        cpu.PC += 2;
        cpu.A = Byte{0x00};
        cpu.setFlags("ZnHC");
    });
}

#define SUB(opcode, reg)                            \
    TEST_F(ALU, SUB_##opcode##_A_##reg) {           \
        setNextInstruction(opcode);                 \
        cpu.A              = Byte{0x05};            \
        cpu.reg            = Byte{0x06};            \
        Byte expectedValue = cpu.A - cpu.reg;       \
        runAndCheck([&expectedValue](Cpu& cpu) {    \
            cpu.PC += 1;                            \
            cpu.A = expectedValue;                  \
            cpu.setFlag(Cpu::Flag::Z, #reg == "A"); \
            cpu.setFlag(Cpu::Flag::N, true);        \
            cpu.setFlag(Cpu::Flag::H, #reg == "A"); \
            cpu.setFlag(Cpu::Flag::C, #reg == "A"); \
        });                                         \
    }                                               \
    TEST_F(ALU, SUB_##opcode##_A_##reg##_toZero) {  \
        setNextInstruction(opcode);                 \
        cpu.A   = Byte{0x80};                       \
        cpu.reg = Byte{0x80};                       \
        cpu.setFlag(Cpu::Flag::Z, false);           \
        runAndCheck([](Cpu& cpu) {                  \
            cpu.PC += 1;                            \
            cpu.A = Byte{0x00};                     \
            cpu.setFlags("ZNHC");                   \
        });                                         \
    }

SUB(0x97, A);
SUB(0x90, B);
SUB(0x91, C);
SUB(0x92, D);
SUB(0x93, E);
SUB(0x94, H);
SUB(0x95, L);
TEST_F(ALU, SUB_A__HL__) {
    setNextInstruction(0x96);
    cpu.A              = Byte{0xFF};
    cpu.HL             = Word{0xFF10};
    cpu.memory[0xFF10] = Byte{0x67};
    cpu.setFlag(Cpu::Flag::C, false);
    cpu.setFlag(Cpu::Flag::H, false);
    runAndCheck([](Cpu& cpu) {
        cpu.PC += 1;
        cpu.A = Byte{0xFF - 0x67};
        cpu.setFlags("zNHC");
    });
}
TEST_F(ALU, SUB_A_IMM) {
    setNextInstruction(0xD6, Byte{0x01});
    cpu.A = Byte{0x00};
    cpu.setFlag(Cpu::Flag::C, true);
    cpu.setFlag(Cpu::Flag::H, true);
    runAndCheck([](Cpu& cpu) {
        cpu.PC += 2;
        cpu.A = Byte{0xFF};
        cpu.setFlags("zNhc");
    });
}
