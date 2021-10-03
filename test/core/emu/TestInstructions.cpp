#include <core/emu/Cpu.h>
#include <gtest/gtest.h>

class TestInstruction : public ::testing::Test {
   protected:
    TestInstruction();
    void setNextInstruction(Byte opcode);
    void setNextInstruction(Byte opcode, Byte data);
    void setNextInstruction(Byte opcode, Word data);
    void runAndCheck(std::function<void(Cpu&)>&& setupExpected);

   protected:
    Cpu cpu;
};

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

class LD : public TestInstruction {};

#define LD_REG_IMM(opcode, reg, value)      \
    TEST_F(LD, LD_##opcode##_##reg##_imm) { \
        setNextInstruction(opcode, value);  \
        runAndCheck([](Cpu& cpu) {          \
            cpu.PC += sizeof(value) + 1;    \
            cpu.reg = value;                \
        });                                 \
    }

LD_REG_IMM(0x3E, A, Byte{0x04});
LD_REG_IMM(0x06, B, Byte{0x04});
LD_REG_IMM(0x0E, C, Byte{0x04});
LD_REG_IMM(0x16, D, Byte{0x04});
LD_REG_IMM(0x1E, E, Byte{0x04});
LD_REG_IMM(0x26, H, Byte{0x04});
LD_REG_IMM(0x2E, L, Byte{0x04});

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

#define LD_REG16_REG16(opcode, dst, src)      \
    TEST_F(LD, LD_##opcode##_##dst##_##src) { \
        setNextInstruction(opcode);           \
        cpu.dst = Word{0x00};                 \
        cpu.src = Word{0x6789};               \
        runAndCheck([](Cpu& cpu) {            \
            cpu.PC += 1;                      \
            cpu.dst = Word{0x6789};           \
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

TEST_F(LD, LD__HLD__A) {
    setNextInstruction(0x32);
    cpu.A            = Byte{0x67};
    cpu.HL           = Word{0x10};
    cpu.memory[0x10] = Byte{0x00};
    runAndCheck([](Cpu& cpu) {
        cpu.PC += 1;
        cpu.HL           = cpu.HL - Word{1};
        cpu.memory[0x10] = Byte{0x67};
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

TEST_F(LD, LD__HLI__A) {
    setNextInstruction(0x22);
    cpu.A            = Byte{0x67};
    cpu.HL           = Word{0x10};
    cpu.memory[0x10] = Byte{0x00};
    runAndCheck([](Cpu& cpu) {
        cpu.PC += 1;
        cpu.HL           = cpu.HL + Word{1};
        cpu.memory[0x10] = Byte{0x67};
    });
}

TEST_F(LD, LD__IMMaddrRel__A) {  // LD (n), A
    setNextInstruction(0xE0, Byte{0x10});
    cpu.A              = Byte{0x67};
    cpu.memory[0xFF10] = Byte{0x00};
    runAndCheck([](Cpu& cpu) {
        cpu.PC += 2;
        cpu.memory[0xFF10] = Byte{0x67};
    });
}

TEST_F(LD, LD_A__IMMaddrRel_) {  // LD A, (n)
    setNextInstruction(0xF0, Byte{0x10});
    cpu.memory[0xFF10] = Byte{0x67};
    cpu.A              = Byte{0x00};
    runAndCheck([](Cpu& cpu) {
        cpu.PC += 2;
        cpu.A = Byte{0x67};
    });
}

LD_REG_IMM(0x01, BC, Word{0x6789});
LD_REG_IMM(0x11, DE, Word{0x6789});
LD_REG_IMM(0x21, HL, Word{0x6789});
LD_REG_IMM(0x31, SP, Word{0x6789});

LD_REG16_REG16(0xF9, SP, HL);

class TestCarry : public ::testing::Test, public Cpu {};

TEST_F(TestCarry, onAdd) {
    setFlag(Cpu::Flag::H, true);
    setFlag(Cpu::Flag::C, true);
    EXPECT_EQ(Byte{0x00}, sum(Byte{0x00}, Byte{0x00}));
    EXPECT_FALSE(getFlag(Cpu::Flag::H));
    EXPECT_FALSE(getFlag(Cpu::Flag::C));

    setFlag(Cpu::Flag::H, true);
    setFlag(Cpu::Flag::C, true);
    EXPECT_EQ(Byte{0x01}, sum(Byte{0x00}, Byte{0x01}));
    EXPECT_FALSE(getFlag(Cpu::Flag::H));
    EXPECT_FALSE(getFlag(Cpu::Flag::C));

    setFlag(Cpu::Flag::H, false);
    setFlag(Cpu::Flag::C, true);
    EXPECT_EQ(Byte{0x10}, sum(Byte{0x0F}, Byte{0x01}));
    EXPECT_TRUE(getFlag(Cpu::Flag::H));
    EXPECT_FALSE(getFlag(Cpu::Flag::C));

    setFlag(Cpu::Flag::H, false);
    setFlag(Cpu::Flag::C, true);
    EXPECT_EQ(Byte{0x10}, sum(Byte{0x0A}, Byte{0x06}));
    EXPECT_TRUE(getFlag(Cpu::Flag::H));
    EXPECT_FALSE(getFlag(Cpu::Flag::C));

    setFlag(Cpu::Flag::H, true);
    setFlag(Cpu::Flag::C, true);
    EXPECT_EQ(Byte{0xF1}, sum(Byte{0xF0}, Byte{0x01}));
    EXPECT_FALSE(getFlag(Cpu::Flag::H));
    EXPECT_FALSE(getFlag(Cpu::Flag::C));

    setFlag(Cpu::Flag::H, true);
    setFlag(Cpu::Flag::C, false);
    EXPECT_EQ(Byte{0x01}, sum(Byte{0xF0}, Byte{0x11}));
    EXPECT_FALSE(getFlag(Cpu::Flag::H));
    EXPECT_TRUE(getFlag(Cpu::Flag::C));

    setFlag(Cpu::Flag::H, false);
    setFlag(Cpu::Flag::C, false);
    EXPECT_EQ(Byte{0x00}, sum(Byte{0xFB}, Byte{0x05}));
    EXPECT_TRUE(getFlag(Cpu::Flag::H));
    EXPECT_TRUE(getFlag(Cpu::Flag::C));

    setFlag(Cpu::Flag::H, false);
    setFlag(Cpu::Flag::C, false);
    EXPECT_EQ(Byte{0xFE}, sum(Byte{0xFF}, Byte{0xFF}));
    EXPECT_TRUE(getFlag(Cpu::Flag::H));
    EXPECT_TRUE(getFlag(Cpu::Flag::C));
}

TEST_F(TestCarry, onSub) {
    setFlag(Cpu::Flag::H, true);
    setFlag(Cpu::Flag::C, true);
    EXPECT_EQ(Byte{0x00}, sub(Byte{0x00}, Byte{0x00}));
    EXPECT_FALSE(getFlag(Cpu::Flag::H));
    EXPECT_FALSE(getFlag(Cpu::Flag::C));

    setFlag(Cpu::Flag::H, false);
    setFlag(Cpu::Flag::C, false);
    EXPECT_EQ(Byte{0xFF}, sub(Byte{0x00}, Byte{0x01}));
    EXPECT_TRUE(getFlag(Cpu::Flag::H));
    EXPECT_TRUE(getFlag(Cpu::Flag::C));

    setFlag(Cpu::Flag::H, true);
    setFlag(Cpu::Flag::C, true);
    EXPECT_EQ(Byte{0x01}, sub(Byte{0x02}, Byte{0x01}));
    EXPECT_FALSE(getFlag(Cpu::Flag::H));
    EXPECT_FALSE(getFlag(Cpu::Flag::C));

    setFlag(Cpu::Flag::H, true);
    setFlag(Cpu::Flag::C, false);
    EXPECT_EQ(Byte{0xF2}, sub(Byte{0x02}, Byte{0x10}));
    EXPECT_FALSE(getFlag(Cpu::Flag::H));
    EXPECT_TRUE(getFlag(Cpu::Flag::C));

    setFlag(Cpu::Flag::H, false);
    setFlag(Cpu::Flag::C, true);
    EXPECT_EQ(Byte{0x8E}, sub(Byte{0xA2}, Byte{0x14}));
    EXPECT_TRUE(getFlag(Cpu::Flag::H));
    EXPECT_FALSE(getFlag(Cpu::Flag::C));

    setFlag(Cpu::Flag::H, true);
    setFlag(Cpu::Flag::C, true);
    EXPECT_EQ(Byte{0x00}, sub(Byte{0xFF}, Byte{0xFF}));
    EXPECT_FALSE(getFlag(Cpu::Flag::H));
    EXPECT_FALSE(getFlag(Cpu::Flag::C));
}

TEST_F(TestCarry, onAdd16) {
    setFlag(Cpu::Flag::H, true);
    setFlag(Cpu::Flag::C, true);
    EXPECT_EQ(Word{0x0000}, sum(Word{0x0000}, Word{0x0000}));
    EXPECT_FALSE(getFlag(Cpu::Flag::H));
    EXPECT_FALSE(getFlag(Cpu::Flag::C));

    setFlag(Cpu::Flag::H, true);
    setFlag(Cpu::Flag::C, true);
    EXPECT_EQ(Word{0x0100}, sum(Word{0x0000}, Word{0x0100}));
    EXPECT_FALSE(getFlag(Cpu::Flag::H));
    EXPECT_FALSE(getFlag(Cpu::Flag::C));

    setFlag(Cpu::Flag::H, false);
    setFlag(Cpu::Flag::C, true);
    EXPECT_EQ(Word{0x1000}, sum(Word{0x0F00}, Word{0x0100}));
    EXPECT_TRUE(getFlag(Cpu::Flag::H));
    EXPECT_FALSE(getFlag(Cpu::Flag::C));

    setFlag(Cpu::Flag::H, false);
    setFlag(Cpu::Flag::C, true);
    EXPECT_EQ(Word{0x1000}, sum(Word{0x0A00}, Word{0x0600}));
    EXPECT_TRUE(getFlag(Cpu::Flag::H));
    EXPECT_FALSE(getFlag(Cpu::Flag::C));

    setFlag(Cpu::Flag::H, true);
    setFlag(Cpu::Flag::C, true);
    EXPECT_EQ(Word{0xF100}, sum(Word{0xF000}, Word{0x0100}));
    EXPECT_FALSE(getFlag(Cpu::Flag::H));
    EXPECT_FALSE(getFlag(Cpu::Flag::C));

    setFlag(Cpu::Flag::H, true);
    setFlag(Cpu::Flag::C, false);
    EXPECT_EQ(Word{0x0100}, sum(Word{0xF000}, Word{0x1100}));
    EXPECT_FALSE(getFlag(Cpu::Flag::H));
    EXPECT_TRUE(getFlag(Cpu::Flag::C));

    setFlag(Cpu::Flag::H, false);
    setFlag(Cpu::Flag::C, false);
    EXPECT_EQ(Word{0x0000}, sum(Word{0xFB00}, Word{0x0500}));
    EXPECT_TRUE(getFlag(Cpu::Flag::H));
    EXPECT_TRUE(getFlag(Cpu::Flag::C));

    setFlag(Cpu::Flag::H, false);
    setFlag(Cpu::Flag::C, false);
    EXPECT_EQ(Word{0xFE00}, sum(Word{0xFF00}, Word{0xFF00}));
    EXPECT_TRUE(getFlag(Cpu::Flag::H));
    EXPECT_TRUE(getFlag(Cpu::Flag::C));

    setFlag(Cpu::Flag::H, true);
    setFlag(Cpu::Flag::C, true);
    EXPECT_EQ(Word{0xFF00}, sum(Word{0xFEFF}, Word{0x0001}));
    EXPECT_FALSE(getFlag(Cpu::Flag::H));
    EXPECT_FALSE(getFlag(Cpu::Flag::C));

    setFlag(Cpu::Flag::H, false);
    setFlag(Cpu::Flag::C, false);
    EXPECT_EQ(Word{0x0000}, sum(Word{0xFFFF}, Word{0x0001}));
    EXPECT_TRUE(getFlag(Cpu::Flag::H));
    EXPECT_TRUE(getFlag(Cpu::Flag::C));

    setFlag(Cpu::Flag::H, true);
    setFlag(Cpu::Flag::C, true);
    EXPECT_EQ(Word{0x0100}, sum(Word{0x00F0}, Word{0x0010}));
    EXPECT_FALSE(getFlag(Cpu::Flag::H));
    EXPECT_FALSE(getFlag(Cpu::Flag::C));
}

TEST_F(TestCarry, onSub16) {
    setFlag(Cpu::Flag::H, true);
    setFlag(Cpu::Flag::C, true);
    EXPECT_EQ(Word{0x0000}, sub(Word{0x0000}, Word{0x0000}));
    EXPECT_FALSE(getFlag(Cpu::Flag::H));
    EXPECT_FALSE(getFlag(Cpu::Flag::C));

    setFlag(Cpu::Flag::H, false);
    setFlag(Cpu::Flag::C, false);
    EXPECT_EQ(Word{0xFF00}, sub(Word{0x0000}, Word{0x0100}));
    EXPECT_TRUE(getFlag(Cpu::Flag::H));
    EXPECT_TRUE(getFlag(Cpu::Flag::C));

    setFlag(Cpu::Flag::H, true);
    setFlag(Cpu::Flag::C, true);
    EXPECT_EQ(Word{0x0100}, sub(Word{0x0200}, Word{0x0100}));
    EXPECT_FALSE(getFlag(Cpu::Flag::H));
    EXPECT_FALSE(getFlag(Cpu::Flag::C));

    setFlag(Cpu::Flag::H, true);
    setFlag(Cpu::Flag::C, false);
    EXPECT_EQ(Word{0xF200}, sub(Word{0x0200}, Word{0x1000}));
    EXPECT_FALSE(getFlag(Cpu::Flag::H));
    EXPECT_TRUE(getFlag(Cpu::Flag::C));

    setFlag(Cpu::Flag::H, false);
    setFlag(Cpu::Flag::C, true);
    EXPECT_EQ(Word{0x8E00}, sub(Word{0xA200}, Word{0x1400}));
    EXPECT_TRUE(getFlag(Cpu::Flag::H));
    EXPECT_FALSE(getFlag(Cpu::Flag::C));

    setFlag(Cpu::Flag::H, true);
    setFlag(Cpu::Flag::C, true);
    EXPECT_EQ(Word{0x0000}, sub(Word{0xFF00}, Word{0xFF00}));
    EXPECT_FALSE(getFlag(Cpu::Flag::H));
    EXPECT_FALSE(getFlag(Cpu::Flag::C));

    setFlag(Cpu::Flag::H, true);
    setFlag(Cpu::Flag::C, true);
    EXPECT_EQ(Word{0xFEF0}, sub(Word{0xFF10}, Word{0x0020}));
    EXPECT_FALSE(getFlag(Cpu::Flag::H));
    EXPECT_FALSE(getFlag(Cpu::Flag::C));

    setFlag(Cpu::Flag::H, false);
    setFlag(Cpu::Flag::C, true);
    EXPECT_EQ(Word{0xEFF0}, sub(Word{0xF010}, Word{0x0020}));
    EXPECT_TRUE(getFlag(Cpu::Flag::H));
    EXPECT_FALSE(getFlag(Cpu::Flag::C));

    setFlag(Cpu::Flag::H, false);
    setFlag(Cpu::Flag::C, false);
    EXPECT_EQ(Word{0xFFF0}, sub(Word{0x0010}, Word{0x0020}));
    EXPECT_TRUE(getFlag(Cpu::Flag::H));
    EXPECT_TRUE(getFlag(Cpu::Flag::C));
}

TEST_F(LD, LDHL_SP_IMM) {
    setNextInstruction(0xF8, Byte{0x14});
    cpu.HL = Word{0x00};
    cpu.SP = Word{0xFFF0};
    cpu.setFlags("ZNhc");
    runAndCheck([](Cpu& cpu) {
        cpu.PC += 2;
        cpu.setFlags("znHC");
        cpu.HL = Word{0x0004};
    });
}