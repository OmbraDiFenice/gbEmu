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

    for (size_t addr = 0; addr < Cpu::kMemSize; ++addr) {
        EXPECT_EQ(expectedCpu.memory[addr], cpu.memory[addr])
            << "memory values differ at address: " << addr;
    }
}

class LD : public TestInstruction {};

#define LD_REG8_IMM(opcode, reg, value)          \
    TEST_F(LD, LD_##opcode##_##reg##_imm) {      \
        setNextInstruction(opcode, Byte{value}); \
        runAndCheck([](Cpu& cpu) {               \
            cpu.PC += 2;                         \
            cpu.reg = Byte{value};               \
        });                                      \
    }

LD_REG8_IMM(0x3E, A, 0x04);
LD_REG8_IMM(0x06, B, 0x04);
LD_REG8_IMM(0x0E, C, 0x04);
LD_REG8_IMM(0x16, D, 0x04);
LD_REG8_IMM(0x1E, E, 0x04);
LD_REG8_IMM(0x26, H, 0x04);
LD_REG8_IMM(0x2E, L, 0x04);

#define LD_REG_REG(opcode, dst, src)          \
    TEST_F(LD, LD_##opcode##_##dst##_##src) { \
        setNextInstruction(opcode);           \
        cpu.dst = Byte{0x00};                 \
        cpu.src = Byte{0x67};                 \
        runAndCheck([](Cpu& cpu) {            \
            cpu.PC += 1;                      \
            cpu.dst = Byte{0x67};             \
        });                                   \
    }

#define LD_REG_REGaddr(opcode, dst, regaddr)           \
    TEST_F(LD, LD_##opcode##_##dst##__##regaddr##__) { \
        setNextInstruction(opcode);                    \
        cpu.dst                 = Byte{0x00};          \
        cpu.regaddr             = Word{0x10A0};        \
        cpu.memory[cpu.regaddr] = Byte{0x67};          \
        runAndCheck([](Cpu& cpu) {                     \
            cpu.PC += 1;                               \
            cpu.dst = Byte{0x67};                      \
        });                                            \
    }

#define LD_REGaddr_REG(opcode, regaddr, src)        \
    TEST_F(LD, LD_##opcode##__##regaddr##__##src) { \
        setNextInstruction(opcode);                 \
        cpu.regaddr             = Word{0x10A0};     \
        cpu.src                 = Byte{0x67};       \
        cpu.memory[cpu.regaddr] = Byte{0x00};       \
        runAndCheck([](Cpu& cpu) {                  \
            cpu.PC += 1;                            \
            cpu.memory[cpu.regaddr] = Byte{0x67};   \
        });                                         \
    }

#define LD_REGaddr_IMM(opcode, regaddr, value)   \
    TEST_F(LD, LD_##opcode##__##regaddr##_imm) { \
        setNextInstruction(opcode);              \
        cpu.regaddr             = Word{0x10A0};  \
        cpu.memory[cpu.regaddr] = Byte{0x00};    \
        cpu.memory[cpu.PC + 1]  = value;         \
        runAndCheck([](Cpu& cpu) {               \
            cpu.PC += 2;                         \
            cpu.memory[0x10A0] = Byte{value};    \
        });                                      \
    }

#define LD_REG_IMMaddr(opcode, reg, immaddr)            \
    TEST_F(LD, LD_##opcode##__##regaddr##_immaddr) {    \
        setNextInstruction(opcode);                     \
        cpu.memory[cpu.PC + 1] = (immaddr >> 0) & 0xFF; \
        cpu.memory[cpu.PC + 2] = (immaddr >> 8) & 0xFF; \
        cpu.memory[immaddr]    = Byte{0x67};            \
        cpu.reg                = Byte{0x00};            \
        runAndCheck([](Cpu& cpu) {                      \
            cpu.PC += 3;                                \
            cpu.reg = Byte{0x67};                       \
        });                                             \
    }

#define LD_IMMaddr_REG(opcode, reg)                  \
    TEST_F(LD, LD_##opcode##__##regaddr##_immaddr) { \
        setNextInstruction(opcode);                  \
        Word addr              = Word{0x10A0};       \
        cpu.memory[cpu.PC + 1] = (addr >> 0) & 0xFF; \
        cpu.memory[cpu.PC + 2] = (addr >> 8) & 0xFF; \
        cpu.reg                = Byte{0x67};         \
        cpu.memory[addr]       = Byte{0x00};         \
        runAndCheck([&addr](Cpu& cpu) {              \
            cpu.PC += 3;                             \
            cpu.memory[addr] = Byte{0x67};           \
        });                                          \
    }

LD_REG_REG(0x7F, A, A);
LD_REG_REG(0x78, A, B);
LD_REG_REG(0x79, A, C);
LD_REG_REG(0x7A, A, D);
LD_REG_REG(0x7B, A, E);
LD_REG_REG(0x7C, A, H);
LD_REG_REG(0x7D, A, L);
LD_REG_REGaddr(0x7E, A, HL);

LD_REG_REG(0x40, B, B);
LD_REG_REG(0x41, B, C);
LD_REG_REG(0x42, B, D);
LD_REG_REG(0x43, B, E);
LD_REG_REG(0x44, B, H);
LD_REG_REG(0x45, B, L);
LD_REG_REGaddr(0x46, B, HL);

LD_REG_REG(0x48, C, B);
LD_REG_REG(0x49, C, C);
LD_REG_REG(0x4A, C, D);
LD_REG_REG(0x4B, C, E);
LD_REG_REG(0x4C, C, H);
LD_REG_REG(0x4D, C, L);
LD_REG_REGaddr(0x4E, C, HL);

LD_REG_REG(0x50, D, B);
LD_REG_REG(0x51, D, C);
LD_REG_REG(0x52, D, D);
LD_REG_REG(0x53, D, E);
LD_REG_REG(0x54, D, H);
LD_REG_REG(0x55, D, L);
LD_REG_REGaddr(0x56, D, HL);

LD_REG_REG(0x58, E, B);
LD_REG_REG(0x59, E, C);
LD_REG_REG(0x5A, E, D);
LD_REG_REG(0x5B, E, E);
LD_REG_REG(0x5C, E, H);
LD_REG_REG(0x5D, E, L);
LD_REG_REGaddr(0x5E, E, HL);

LD_REG_REG(0x60, H, B);
LD_REG_REG(0x61, H, C);
LD_REG_REG(0x62, H, D);
LD_REG_REG(0x63, H, E);
LD_REG_REG(0x64, H, H);
LD_REG_REG(0x65, H, L);
LD_REG_REGaddr(0x66, H, HL);

LD_REG_REG(0x68, L, B);
LD_REG_REG(0x69, L, C);
LD_REG_REG(0x6A, L, D);
LD_REG_REG(0x6B, L, E);
LD_REG_REG(0x6C, L, H);
LD_REG_REG(0x6D, L, L);
LD_REG_REGaddr(0x6E, L, HL);

LD_REGaddr_REG(0x70, HL, B);
LD_REGaddr_REG(0x71, HL, C);
LD_REGaddr_REG(0x72, HL, D);
LD_REGaddr_REG(0x73, HL, E);
LD_REGaddr_REG(0x74, HL, H);
LD_REGaddr_REG(0x75, HL, L);
LD_REGaddr_IMM(0x36, HL, 0x67);

LD_REG_REGaddr(0x0A, A, BC);
LD_REG_REGaddr(0x1A, A, DE);

LD_REG_IMMaddr(0xFA, A, 0x10A0);

LD_REG_REG(0x47, B, A);
LD_REG_REG(0x4F, C, A);
LD_REG_REG(0x57, D, A);
LD_REG_REG(0x5F, E, A);
LD_REG_REG(0x67, H, A);
LD_REG_REG(0x6F, L, A);

LD_REGaddr_REG(0x02, BC, A);
LD_REGaddr_REG(0x12, DE, A);
LD_REGaddr_REG(0x77, HL, A);

LD_IMMaddr_REG(0xEA, A);

TEST_F(LD, LD_A__C_) {
    setNextInstruction(0xF2);
    cpu.C              = Byte{0x01};
    cpu.memory[0xFF01] = Byte{0x67};
    cpu.A              = Byte{0x00};
    runAndCheck([](Cpu& cpu) {
        cpu.PC += 1;
        cpu.A = Byte{0x67};
    });
}

TEST_F(LD, LD__C__A) {
    setNextInstruction(0xE2);
    cpu.C              = Byte(0x10);
    cpu.memory[0xFF10] = Byte{0x00};
    cpu.A              = Byte{0x67};
    runAndCheck([](Cpu& cpu) {
        cpu.PC += 1;
        cpu.memory[0xFF10] = Byte{0x67};
    });
}

TEST_F(LD, LD_A__HLD__) {
    setNextInstruction(0x3A);
    cpu.HL             = Word{0xFF10};
    cpu.memory[0xFF10] = Byte{0x67};
    cpu.A              = Byte{0x00};
    runAndCheck([](Cpu& cpu) {
        cpu.PC += 1;
        cpu.A  = Byte{0x67};
        cpu.HL = cpu.HL - Word{1};
    });
}

TEST_F(LD, LD_A__HLI__) {
    setNextInstruction(0x2A);
    cpu.HL           = Word{0x10};
    cpu.memory[0x10] = Byte{0x67};
    cpu.A            = Byte{0x00};
    runAndCheck([](Cpu& cpu) {
        cpu.PC += 1;
        cpu.A  = Byte{0x67};
        cpu.HL = cpu.HL + Word{1};
    });
}
