#include <core/emu/TestInstructions.h>
#include <gtest/gtest.h>

class TestJumps : public TestInstruction {};

TEST_F(TestJumps, JP) {
    setNextInstruction(0xC3, Word{0xFF00});
    runAndCheck([](Cpu& cpu) { cpu.PC = Word{0xFF00}; });

    setNextInstruction(0xC3, Word{0x0F00});
    runAndCheck([](Cpu& cpu) { cpu.PC = Word{0x0F00}; });
}

#define CONDITIONAL_JUMP(opcode, condition, flag)                            \
    TEST_F(TestJumps, JP_##opcode##_cond_##flag##_IMMaddr_conditionMet) {    \
        setNextInstruction(opcode, Word{0xFF00});                            \
        cpu.setFlag(Cpu::Flag::flag, condition);                             \
        runAndCheck([](Cpu& cpu) { cpu.PC = Word{0xFF00}; });                \
    }                                                                        \
                                                                             \
    TEST_F(TestJumps, JP_##opcode##_cond_##flag##_IMMaddr_conditionNotMet) { \
        setNextInstruction(opcode, Word{0xFF00});                            \
        cpu.setFlag(Cpu::Flag::flag, !condition);                            \
        runAndCheck([](Cpu& cpu) { cpu.PC += 3; });                          \
    }

CONDITIONAL_JUMP(0xC2, false, Z);
CONDITIONAL_JUMP(0xCA, true, Z);
CONDITIONAL_JUMP(0xD2, false, C);
CONDITIONAL_JUMP(0xDA, true, C);

TEST_F(TestJumps, JP__HL__) {
    setNextInstruction(0xE9);
    cpu.HL = Word{0xFF10};
    runAndCheck([](Cpu& cpu) { cpu.PC = Word{0xFF10}; });

    setNextInstruction(0xE9);
    cpu.HL = Word{0x0010};
    runAndCheck([](Cpu& cpu) { cpu.PC = Word{0x0010}; });
}

TEST_F(TestJumps, JR_IMMoffset) {
    setNextInstruction(0x18, Byte{0x10});
    runAndCheck([](Cpu& cpu) { cpu.PC += 2 + Byte{0x10}; });
}

#define CONDITIONAL_RELATIVE_JUMP(opcode, condition, flag)                   \
    TEST_F(TestJumps, JR_##opcode##_cond_##flag##_IMMaddr_conditionMet) {    \
        setNextInstruction(opcode, Byte{0x10});                              \
        cpu.setFlag(Cpu::Flag::flag, condition);                             \
        runAndCheck([](Cpu& cpu) { cpu.PC += 2 + Byte{0x10}; });             \
    }                                                                        \
                                                                             \
    TEST_F(TestJumps, JR_##opcode##_cond_##flag##_IMMaddr_conditionNotMet) { \
        setNextInstruction(opcode, Byte{0x20});                              \
        cpu.setFlag(Cpu::Flag::flag, !condition);                            \
        runAndCheck([](Cpu& cpu) { cpu.PC += 2; });                          \
    }

CONDITIONAL_RELATIVE_JUMP(0x20, false, Z);
CONDITIONAL_RELATIVE_JUMP(0x28, true, Z);
CONDITIONAL_RELATIVE_JUMP(0x30, false, C);
CONDITIONAL_RELATIVE_JUMP(0x38, true, C);
