#include <core/emu/TestInstructions.h>
#include <gtest/gtest.h>

class TestCalls : public TestInstruction {
   public:
    TestCalls() {
        cpu.PC = kStartingPC;
        cpu.SP = kStartingSP;
    }

   protected:
    static const Word kStartingPC = Word{0x5510};
    static const Word kStartingSP = Word{0x1100};
};

TEST_F(TestCalls, CALL_IMM) {
    setNextInstruction(0xCD, Word{0xFF11});
    runAndCheck([](Cpu& cpu) {
        cpu.SP -= 2;
        cpu.memory[kStartingSP - 1] = (cpu.PC + 3) >> 8;
        cpu.memory[kStartingSP - 2] = (cpu.PC + 3) & 0xFF;
        cpu.PC                      = Word{0xFF11};
    });
}

#define CONDITIONAL_CALL(opcode, condition, flag)                            \
    TEST_F(TestCalls,                                                        \
           CALL_##opcode##_##condition##_##flag##_IMMaddr_conditionMet) {    \
        setNextInstruction(opcode, Word{0xFF11});                            \
        cpu.setFlag(Cpu::Flag::flag, condition);                             \
        runAndCheck([](Cpu& cpu) {                                           \
            cpu.SP -= 2;                                                     \
            cpu.memory[kStartingSP - 1] = (cpu.PC + 3) >> 8;                 \
            cpu.memory[kStartingSP - 2] = (cpu.PC + 3) & 0xFF;               \
            cpu.PC                      = Word{0xFF11};                      \
        });                                                                  \
    }                                                                        \
                                                                             \
    TEST_F(TestCalls,                                                        \
           CALL_##opcode##_##condition##_##flag##_IMMaddr_conditionNotMet) { \
        setNextInstruction(opcode, Word{0xFF11});                            \
        cpu.setFlag(Cpu::Flag::flag, !condition);                            \
        runAndCheck([](Cpu& cpu) { cpu.PC += 3; });                          \
    }

CONDITIONAL_CALL(0xC4, false, Z);
CONDITIONAL_CALL(0xCC, true, Z);
CONDITIONAL_CALL(0xD4, false, C);
CONDITIONAL_CALL(0xDC, true, C);

#define RST(opcode, n)                                         \
    TEST_F(TestCalls, RST_##opcode##_##n) {                    \
        setNextInstruction(opcode);                            \
        runAndCheck([](Cpu& cpu) {                             \
            cpu.SP -= 2;                                       \
            cpu.memory[kStartingSP - 1] = (cpu.PC + 1) >> 8;   \
            cpu.memory[kStartingSP - 2] = (cpu.PC + 1) & 0xFF; \
            cpu.PC                      = Word{n};             \
        });                                                    \
    }

RST(0xC7, 0x00);
RST(0xCF, 0x08);
RST(0xD7, 0x10);
RST(0xDF, 0x18);
RST(0xE7, 0x20);
RST(0xEF, 0x28);
RST(0xF7, 0x30);
RST(0xFF, 0x38);

TEST_F(TestCalls, RET) {
    setNextInstruction(0xC9);
    cpu.memory[kStartingSP - 1] = Byte{kStartingPC >> 8};
    cpu.memory[kStartingSP - 2] = Byte{kStartingPC & 0xFF};
    cpu.SP                      = kStartingSP - 2;
    runAndCheck([](Cpu& cpu) {
        cpu.SP = kStartingSP;
        cpu.PC = kStartingPC;
    });
}

#define CONDITIONAL_RET(opcode, condition, flag)                               \
    TEST_F(TestCalls, RET_##opcode##_##condition##_##flag##_conditionMet) {    \
        setNextInstruction(opcode);                                            \
        cpu.memory[kStartingSP - 1] = Byte{kStartingPC >> 8};                  \
        cpu.memory[kStartingSP - 2] = Byte{kStartingPC & 0xFF};                \
        cpu.SP                      = kStartingSP - 2;                         \
        cpu.setFlag(Cpu::Flag::flag, condition);                               \
        runAndCheck([](Cpu& cpu) {                                             \
            cpu.SP = kStartingSP;                                              \
            cpu.PC = kStartingPC;                                              \
        });                                                                    \
    }                                                                          \
                                                                               \
    TEST_F(TestCalls, RET_##opcode##_##condition##_##flag##_conditionNotMet) { \
        setNextInstruction(opcode);                                            \
        cpu.memory[kStartingSP - 1] = Byte{kStartingPC >> 8};                  \
        cpu.memory[kStartingSP - 2] = Byte{kStartingPC & 0xFF};                \
        cpu.SP                      = kStartingSP - 2;                         \
        cpu.setFlag(Cpu::Flag::flag, !condition);                              \
        runAndCheck([](Cpu& cpu) { cpu.PC += 1; });                            \
    }

CONDITIONAL_RET(0xC0, false, Z);
CONDITIONAL_RET(0xC8, true, Z);
CONDITIONAL_RET(0xD0, false, C);
CONDITIONAL_RET(0xD8, true, C);

TEST_F(TestCalls, RETI) {
    setNextInstruction(0xD9);
    cpu.memory[kStartingSP - 1] = Byte{kStartingPC >> 8};
    cpu.memory[kStartingSP - 2] = Byte{kStartingPC & 0xFF};
    cpu.SP                      = kStartingSP - 2;
    cpu.interruptsEnabled       = false;
    runAndCheck([](Cpu& cpu) {
        cpu.SP                = kStartingSP;
        cpu.PC                = kStartingPC;
        cpu.interruptsEnabled = true;
    });
}