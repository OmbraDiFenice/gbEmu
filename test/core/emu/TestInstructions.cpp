#include <Pch.h>

#include <gtest/gtest.h>
#include "TestInstructions.h"

TestInstruction::TestInstruction() {
    cpu.PC = 0;
    cpu.SP = 0;
    cpu.AF = 0;
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

void TestInstruction::setNextInstruction(Byte opcode, Word data) {
    setNextInstruction(opcode);
    cpu.memory[cpu.PC + 1] = (data >> 8) & 0xFF;
    cpu.memory[cpu.PC + 2] = data & 0xFF;
}

void TestInstruction::runAndCheck(std::function<void(Cpu&)>&& setupExpected) {
    Cpu expectedCpu{cpu};
    setupExpected(expectedCpu);
    cpu.tick();

    EXPECT_EQ(expectedCpu.PC, cpu.PC);
    EXPECT_EQ(expectedCpu.SP, cpu.SP);
    EXPECT_EQ(expectedCpu.A, cpu.A);
    EXPECT_EQ(expectedCpu.B, cpu.B);
    EXPECT_EQ(expectedCpu.C, cpu.C);
    EXPECT_EQ(expectedCpu.D, cpu.D);
    EXPECT_EQ(expectedCpu.E, cpu.E);
    EXPECT_EQ(expectedCpu.H, cpu.H);
    EXPECT_EQ(expectedCpu.L, cpu.L);

    for (size_t addr = 0; addr < Cpu::kMemSize; ++addr) {
        EXPECT_EQ(expectedCpu.memory[addr], cpu.memory[addr])
            << "memory values differ at address: " << addr;
    }

    EXPECT_EQ(expectedCpu.F & 0x0F, cpu.F & 0x0F);
    EXPECT_EQ(expectedCpu.getFlag(Cpu::Flag::Z), cpu.getFlag(Cpu::Flag::Z));
    EXPECT_EQ(expectedCpu.getFlag(Cpu::Flag::N), cpu.getFlag(Cpu::Flag::N));
    EXPECT_EQ(expectedCpu.getFlag(Cpu::Flag::H), cpu.getFlag(Cpu::Flag::H));
    EXPECT_EQ(expectedCpu.getFlag(Cpu::Flag::C), cpu.getFlag(Cpu::Flag::C));
}