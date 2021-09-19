#include <core/emu/Cpu.h>
#include <gtest/gtest.h>

class TestInstruction : public ::testing::Test {
   protected:
    TestInstruction();
    void setNextInstruction(Byte opcode);
    void setNextInstruction(Byte opcode, Byte data);
    void runAndCheck(std::function<void(Cpu&)>&& setupExpected);

   protected:
    Cpu cpu;
};

TestInstruction::TestInstruction() {
    cpu.PC = 0;
    cpu.BC = 0;
    cpu.DE = 0;
    cpu.HL = 0;
    std::memset(cpu.memory, 0, Cpu::kMemSize);
}

void TestInstruction::setNextInstruction(Byte opcode) {
    cpu.memory[cpu.PC] = opcode;
}

void TestInstruction::setNextInstruction(Byte opcode, Byte data) {
    setNextInstruction(opcode);
    cpu.memory[cpu.PC + 1] = data;
}

void TestInstruction::runAndCheck(std::function<void(Cpu&)>&& setupExpected) {
    Cpu expectedCpu{cpu};
    cpu.tick();
    setupExpected(expectedCpu);

    EXPECT_EQ(expectedCpu.PC, cpu.PC);
    EXPECT_EQ(expectedCpu.B, cpu.B);
    EXPECT_EQ(expectedCpu.C, cpu.C);
    EXPECT_EQ(expectedCpu.D, cpu.D);
    EXPECT_EQ(expectedCpu.E, cpu.E);
    EXPECT_EQ(expectedCpu.H, cpu.H);
    EXPECT_EQ(expectedCpu.L, cpu.L);

    for (uint32_t addr = 0; addr < Cpu::kMemSize; ++addr) {
        EXPECT_EQ(expectedCpu.memory[addr], cpu.memory[addr])
            << "memory values differ at address: " << addr;
    }
}

class LD : public TestInstruction {};

#define LD_REG8_IMM(opcode, reg, value)          \
    TEST_F(LD, LD_##reg##_imm) {                 \
        setNextInstruction(opcode, Byte{value}); \
        runAndCheck([](Cpu& cpu) {               \
            cpu.PC += 2;                         \
            cpu.reg = Byte{value};               \
        });                                      \
    }

LD_REG8_IMM(0x06, B, 0x04);
LD_REG8_IMM(0x0E, C, 0x04);
LD_REG8_IMM(0x16, D, 0x04);
LD_REG8_IMM(0x1E, E, 0x04);
LD_REG8_IMM(0x26, H, 0x04);
LD_REG8_IMM(0x2E, L, 0x04);
