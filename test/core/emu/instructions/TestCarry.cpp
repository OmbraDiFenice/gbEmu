#include <Pch.h>

#include <gtest/gtest.h>
#include "../TestInstructions.h"

class TestCarry : public TestInstruction {};

TEST_F(TestCarry, onAdd) {
    cpu.setFlag(Cpu::Flag::H, true);
    cpu.setFlag(Cpu::Flag::C, true);
    EXPECT_EQ(Byte{0x00}, cpu.sum(Byte{0x00}, Byte{0x00}));
    EXPECT_FALSE(cpu.getFlag(Cpu::Flag::H));
    EXPECT_FALSE(cpu.getFlag(Cpu::Flag::C));

    cpu.setFlag(Cpu::Flag::H, true);
    cpu.setFlag(Cpu::Flag::C, true);
    EXPECT_EQ(Byte{0x01}, cpu.sum(Byte{0x00}, Byte{0x01}));
    EXPECT_FALSE(cpu.getFlag(Cpu::Flag::H));
    EXPECT_FALSE(cpu.getFlag(Cpu::Flag::C));

    cpu.setFlag(Cpu::Flag::H, false);
    cpu.setFlag(Cpu::Flag::C, true);
    EXPECT_EQ(Byte{0x10}, cpu.sum(Byte{0x0F}, Byte{0x01}));
    EXPECT_TRUE(cpu.getFlag(Cpu::Flag::H));
    EXPECT_FALSE(cpu.getFlag(Cpu::Flag::C));

    cpu.setFlag(Cpu::Flag::H, false);
    cpu.setFlag(Cpu::Flag::C, true);
    EXPECT_EQ(Byte{0x10}, cpu.sum(Byte{0x0A}, Byte{0x06}));
    EXPECT_TRUE(cpu.getFlag(Cpu::Flag::H));
    EXPECT_FALSE(cpu.getFlag(Cpu::Flag::C));

    cpu.setFlag(Cpu::Flag::H, true);
    cpu.setFlag(Cpu::Flag::C, true);
    EXPECT_EQ(Byte{0xF1}, cpu.sum(Byte{0xF0}, Byte{0x01}));
    EXPECT_FALSE(cpu.getFlag(Cpu::Flag::H));
    EXPECT_FALSE(cpu.getFlag(Cpu::Flag::C));

    cpu.setFlag(Cpu::Flag::H, true);
    cpu.setFlag(Cpu::Flag::C, false);
    EXPECT_EQ(Byte{0x01}, cpu.sum(Byte{0xF0}, Byte{0x11}));
    EXPECT_FALSE(cpu.getFlag(Cpu::Flag::H));
    EXPECT_TRUE(cpu.getFlag(Cpu::Flag::C));

    cpu.setFlag(Cpu::Flag::H, false);
    cpu.setFlag(Cpu::Flag::C, false);
    EXPECT_EQ(Byte{0x00}, cpu.sum(Byte{0xFB}, Byte{0x05}));
    EXPECT_TRUE(cpu.getFlag(Cpu::Flag::H));
    EXPECT_TRUE(cpu.getFlag(Cpu::Flag::C));

    cpu.setFlag(Cpu::Flag::H, false);
    cpu.setFlag(Cpu::Flag::C, false);
    EXPECT_EQ(Byte{0xFE}, cpu.sum(Byte{0xFF}, Byte{0xFF}));
    EXPECT_TRUE(cpu.getFlag(Cpu::Flag::H));
    EXPECT_TRUE(cpu.getFlag(Cpu::Flag::C));
}

TEST_F(TestCarry, onSub) {
    cpu.setFlag(Cpu::Flag::H, true);
    cpu.setFlag(Cpu::Flag::C, true);
    EXPECT_EQ(Byte{0x00}, cpu.sub(Byte{0x00}, Byte{0x00}));
    EXPECT_FALSE(cpu.getFlag(Cpu::Flag::H));
    EXPECT_FALSE(cpu.getFlag(Cpu::Flag::C));

    cpu.setFlag(Cpu::Flag::H, false);
    cpu.setFlag(Cpu::Flag::C, false);
    EXPECT_EQ(Byte{0xFF}, cpu.sub(Byte{0x00}, Byte{0x01}));
    EXPECT_TRUE(cpu.getFlag(Cpu::Flag::H));
    EXPECT_TRUE(cpu.getFlag(Cpu::Flag::C));

    cpu.setFlag(Cpu::Flag::H, true);
    cpu.setFlag(Cpu::Flag::C, true);
    EXPECT_EQ(Byte{0x01}, cpu.sub(Byte{0x02}, Byte{0x01}));
    EXPECT_FALSE(cpu.getFlag(Cpu::Flag::H));
    EXPECT_FALSE(cpu.getFlag(Cpu::Flag::C));

    cpu.setFlag(Cpu::Flag::H, true);
    cpu.setFlag(Cpu::Flag::C, false);
    EXPECT_EQ(Byte{0xF2}, cpu.sub(Byte{0x02}, Byte{0x10}));
    EXPECT_FALSE(cpu.getFlag(Cpu::Flag::H));
    EXPECT_TRUE(cpu.getFlag(Cpu::Flag::C));

    cpu.setFlag(Cpu::Flag::H, false);
    cpu.setFlag(Cpu::Flag::C, true);
    EXPECT_EQ(Byte{0x8E}, cpu.sub(Byte{0xA2}, Byte{0x14}));
    EXPECT_TRUE(cpu.getFlag(Cpu::Flag::H));
    EXPECT_FALSE(cpu.getFlag(Cpu::Flag::C));

    cpu.setFlag(Cpu::Flag::H, true);
    cpu.setFlag(Cpu::Flag::C, true);
    EXPECT_EQ(Byte{0x00}, cpu.sub(Byte{0xFF}, Byte{0xFF}));
    EXPECT_FALSE(cpu.getFlag(Cpu::Flag::H));
    EXPECT_FALSE(cpu.getFlag(Cpu::Flag::C));
}

TEST_F(TestCarry, onAdd16) {
    cpu.setFlag(Cpu::Flag::H, true);
    cpu.setFlag(Cpu::Flag::C, true);
    EXPECT_EQ(Word{0x0000}, cpu.sum(Word{0x0000}, Word{0x0000}));
    EXPECT_FALSE(cpu.getFlag(Cpu::Flag::H));
    EXPECT_FALSE(cpu.getFlag(Cpu::Flag::C));

    cpu.setFlag(Cpu::Flag::H, true);
    cpu.setFlag(Cpu::Flag::C, true);
    EXPECT_EQ(Word{0x0100}, cpu.sum(Word{0x0000}, Word{0x0100}));
    EXPECT_FALSE(cpu.getFlag(Cpu::Flag::H));
    EXPECT_FALSE(cpu.getFlag(Cpu::Flag::C));

    cpu.setFlag(Cpu::Flag::H, false);
    cpu.setFlag(Cpu::Flag::C, true);
    EXPECT_EQ(Word{0x1000}, cpu.sum(Word{0x0F00}, Word{0x0100}));
    EXPECT_TRUE(cpu.getFlag(Cpu::Flag::H));
    EXPECT_FALSE(cpu.getFlag(Cpu::Flag::C));

    cpu.setFlag(Cpu::Flag::H, false);
    cpu.setFlag(Cpu::Flag::C, true);
    EXPECT_EQ(Word{0x1000}, cpu.sum(Word{0x0A00}, Word{0x0600}));
    EXPECT_TRUE(cpu.getFlag(Cpu::Flag::H));
    EXPECT_FALSE(cpu.getFlag(Cpu::Flag::C));

    cpu.setFlag(Cpu::Flag::H, true);
    cpu.setFlag(Cpu::Flag::C, true);
    EXPECT_EQ(Word{0xF100}, cpu.sum(Word{0xF000}, Word{0x0100}));
    EXPECT_FALSE(cpu.getFlag(Cpu::Flag::H));
    EXPECT_FALSE(cpu.getFlag(Cpu::Flag::C));

    cpu.setFlag(Cpu::Flag::H, true);
    cpu.setFlag(Cpu::Flag::C, false);
    EXPECT_EQ(Word{0x0100}, cpu.sum(Word{0xF000}, Word{0x1100}));
    EXPECT_FALSE(cpu.getFlag(Cpu::Flag::H));
    EXPECT_TRUE(cpu.getFlag(Cpu::Flag::C));

    cpu.setFlag(Cpu::Flag::H, false);
    cpu.setFlag(Cpu::Flag::C, false);
    EXPECT_EQ(Word{0x0000}, cpu.sum(Word{0xFB00}, Word{0x0500}));
    EXPECT_TRUE(cpu.getFlag(Cpu::Flag::H));
    EXPECT_TRUE(cpu.getFlag(Cpu::Flag::C));

    cpu.setFlag(Cpu::Flag::H, false);
    cpu.setFlag(Cpu::Flag::C, false);
    EXPECT_EQ(Word{0xFE00}, cpu.sum(Word{0xFF00}, Word{0xFF00}));
    EXPECT_TRUE(cpu.getFlag(Cpu::Flag::H));
    EXPECT_TRUE(cpu.getFlag(Cpu::Flag::C));

    cpu.setFlag(Cpu::Flag::H, true);
    cpu.setFlag(Cpu::Flag::C, true);
    EXPECT_EQ(Word{0xFF00}, cpu.sum(Word{0xFEFF}, Word{0x0001}));
    EXPECT_FALSE(cpu.getFlag(Cpu::Flag::H));
    EXPECT_FALSE(cpu.getFlag(Cpu::Flag::C));

    cpu.setFlag(Cpu::Flag::H, false);
    cpu.setFlag(Cpu::Flag::C, false);
    EXPECT_EQ(Word{0x0000}, cpu.sum(Word{0xFFFF}, Word{0x0001}));
    EXPECT_TRUE(cpu.getFlag(Cpu::Flag::H));
    EXPECT_TRUE(cpu.getFlag(Cpu::Flag::C));

    cpu.setFlag(Cpu::Flag::H, true);
    cpu.setFlag(Cpu::Flag::C, true);
    EXPECT_EQ(Word{0x0100}, cpu.sum(Word{0x00F0}, Word{0x0010}));
    EXPECT_FALSE(cpu.getFlag(Cpu::Flag::H));
    EXPECT_FALSE(cpu.getFlag(Cpu::Flag::C));
}

TEST_F(TestCarry, onSub16) {
    cpu.setFlag(Cpu::Flag::H, true);
    cpu.setFlag(Cpu::Flag::C, true);
    EXPECT_EQ(Word{0x0000}, cpu.sub(Word{0x0000}, Word{0x0000}));
    EXPECT_FALSE(cpu.getFlag(Cpu::Flag::H));
    EXPECT_FALSE(cpu.getFlag(Cpu::Flag::C));

    cpu.setFlag(Cpu::Flag::H, false);
    cpu.setFlag(Cpu::Flag::C, false);
    EXPECT_EQ(Word{0xFF00}, cpu.sub(Word{0x0000}, Word{0x0100}));
    EXPECT_TRUE(cpu.getFlag(Cpu::Flag::H));
    EXPECT_TRUE(cpu.getFlag(Cpu::Flag::C));

    cpu.setFlag(Cpu::Flag::H, true);
    cpu.setFlag(Cpu::Flag::C, true);
    EXPECT_EQ(Word{0x0100}, cpu.sub(Word{0x0200}, Word{0x0100}));
    EXPECT_FALSE(cpu.getFlag(Cpu::Flag::H));
    EXPECT_FALSE(cpu.getFlag(Cpu::Flag::C));

    cpu.setFlag(Cpu::Flag::H, true);
    cpu.setFlag(Cpu::Flag::C, false);
    EXPECT_EQ(Word{0xF200}, cpu.sub(Word{0x0200}, Word{0x1000}));
    EXPECT_FALSE(cpu.getFlag(Cpu::Flag::H));
    EXPECT_TRUE(cpu.getFlag(Cpu::Flag::C));

    cpu.setFlag(Cpu::Flag::H, false);
    cpu.setFlag(Cpu::Flag::C, true);
    EXPECT_EQ(Word{0x8E00}, cpu.sub(Word{0xA200}, Word{0x1400}));
    EXPECT_TRUE(cpu.getFlag(Cpu::Flag::H));
    EXPECT_FALSE(cpu.getFlag(Cpu::Flag::C));

    cpu.setFlag(Cpu::Flag::H, true);
    cpu.setFlag(Cpu::Flag::C, true);
    EXPECT_EQ(Word{0x0000}, cpu.sub(Word{0xFF00}, Word{0xFF00}));
    EXPECT_FALSE(cpu.getFlag(Cpu::Flag::H));
    EXPECT_FALSE(cpu.getFlag(Cpu::Flag::C));

    cpu.setFlag(Cpu::Flag::H, true);
    cpu.setFlag(Cpu::Flag::C, true);
    EXPECT_EQ(Word{0xFEF0}, cpu.sub(Word{0xFF10}, Word{0x0020}));
    EXPECT_FALSE(cpu.getFlag(Cpu::Flag::H));
    EXPECT_FALSE(cpu.getFlag(Cpu::Flag::C));

    cpu.setFlag(Cpu::Flag::H, false);
    cpu.setFlag(Cpu::Flag::C, true);
    EXPECT_EQ(Word{0xEFF0}, cpu.sub(Word{0xF010}, Word{0x0020}));
    EXPECT_TRUE(cpu.getFlag(Cpu::Flag::H));
    EXPECT_FALSE(cpu.getFlag(Cpu::Flag::C));

    cpu.setFlag(Cpu::Flag::H, false);
    cpu.setFlag(Cpu::Flag::C, false);
    EXPECT_EQ(Word{0xFFF0}, cpu.sub(Word{0x0010}, Word{0x0020}));
    EXPECT_TRUE(cpu.getFlag(Cpu::Flag::H));
    EXPECT_TRUE(cpu.getFlag(Cpu::Flag::C));
}
