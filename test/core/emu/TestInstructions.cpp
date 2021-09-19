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
    cpu.A  = 0;
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
    setupExpected(expectedCpu);
    cpu.tick();

    EXPECT_EQ(expectedCpu.PC, cpu.PC);
    EXPECT_EQ(expectedCpu.A, cpu.A);
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

#define LD_REG_REG(opcode, dst, src) \
    TEST_F(LD, LD_##dst##_##src) {   \
        setNextInstruction(opcode);  \
        cpu.dst = Byte{0x00};        \
        cpu.src = Byte{0x67};        \
        runAndCheck([](Cpu& cpu) {   \
            cpu.PC += 1;             \
            cpu.dst = Byte{0x67};    \
        });                          \
    }

#define LD_REG_REGaddr(opcode, dst, regaddr)    \
    TEST_F(LD, LD_##dst##__##regaddr__) {       \
        setNextInstruction(opcode);             \
        cpu.dst                 = Byte{0x00};   \
        cpu.regaddr             = Word{0x10A0}; \
        cpu.memory[cpu.regaddr] = Byte{0x67};   \
        runAndCheck([](Cpu& cpu) {              \
            cpu.PC += 1;                        \
            cpu.dst = Byte{0x67};               \
        });                                     \
    }

#define LD_REGaddr_REG(opcode, regaddr, src)      \
    TEST_F(LD, LD__##regaddr##__##src) {          \
        setNextInstruction(opcode);               \
        cpu.regaddr             = Word{0x10A0};   \
        cpu.src                 = Byte{0x67};     \
        cpu.memory[cpu.regaddr] = Byte{0x00};     \
        runAndCheck([](Cpu& cpu) {                \
            cpu.PC += 1;                          \
            cpu.memory[cpu.regaddr] = Byte{0x67}; \
        });                                       \
    }

#define LD_REGaddr_IMM(opcode, regaddr, value)  \
    TEST_F(LD, LD__##regaddr##_imm) {           \
        setNextInstruction(opcode);             \
        cpu.regaddr             = Word{0x10A0}; \
        cpu.memory[cpu.regaddr] = Byte{0x00};   \
        cpu.memory[cpu.PC + 1]  = value;        \
        runAndCheck([](Cpu& cpu) {              \
            cpu.PC += 2;                        \
            cpu.memory[0x10A0] = Byte{value};   \
        });                                     \
    }

LD_REG_REG(0X7F, A, A);
LD_REG_REG(0X78, A, B);
LD_REG_REG(0X79, A, C);
LD_REG_REG(0X7A, A, D);
LD_REG_REG(0X7B, A, E);
LD_REG_REG(0X7C, A, H);
LD_REG_REG(0X7D, A, L);
LD_REG_REGaddr(0x7E, A, HL);

LD_REG_REG(0X40, B, B);
LD_REG_REG(0X41, B, C);
LD_REG_REG(0X42, B, D);
LD_REG_REG(0X43, B, E);
LD_REG_REG(0X44, B, H);
LD_REG_REG(0X45, B, L);
LD_REG_REGaddr(0x46, B, HL);

LD_REG_REG(0X48, C, B);
LD_REG_REG(0X49, C, C);
LD_REG_REG(0X4A, C, D);
LD_REG_REG(0X4B, C, E);
LD_REG_REG(0X4C, C, H);
LD_REG_REG(0X4D, C, L);
LD_REG_REGaddr(0x4E, C, HL);

LD_REG_REG(0X50, D, B);
LD_REG_REG(0X51, D, C);
LD_REG_REG(0X52, D, D);
LD_REG_REG(0X53, D, E);
LD_REG_REG(0X54, D, H);
LD_REG_REG(0X55, D, L);
LD_REG_REGaddr(0x56, D, HL);

LD_REG_REG(0X58, E, B);
LD_REG_REG(0X59, E, C);
LD_REG_REG(0X5A, E, D);
LD_REG_REG(0X5B, E, E);
LD_REG_REG(0X5C, E, H);
LD_REG_REG(0X5D, E, L);
LD_REG_REGaddr(0x5E, E, HL);

LD_REG_REG(0X60, H, B);
LD_REG_REG(0X61, H, C);
LD_REG_REG(0X62, H, D);
LD_REG_REG(0X63, H, E);
LD_REG_REG(0X64, H, H);
LD_REG_REG(0X65, H, L);
LD_REG_REGaddr(0x66, H, HL);

LD_REG_REG(0X68, L, B);
LD_REG_REG(0X69, L, C);
LD_REG_REG(0X6A, L, D);
LD_REG_REG(0X6B, L, E);
LD_REG_REG(0X6C, L, H);
LD_REG_REG(0X6D, L, L);
LD_REG_REGaddr(0x6E, L, HL);

LD_REGaddr_REG(0X70, HL, B);
LD_REGaddr_REG(0X71, HL, C);
LD_REGaddr_REG(0X72, HL, D);
LD_REGaddr_REG(0X73, HL, E);
LD_REGaddr_REG(0X74, HL, H);
LD_REGaddr_REG(0X75, HL, L);
LD_REGaddr_IMM(0x36, HL, 0x67);
