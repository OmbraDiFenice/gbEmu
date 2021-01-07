#include <gtest/gtest.h>
#include <core/emu/Memory.h>

class TestMemory : public ::testing::Test {};

TEST_F(TestMemory, writeRead_1byte) {
    Memory mem(1);
    Byte b = 0xAB;
    mem.write(0, b);
    EXPECT_EQ(b, mem.read(0));
}

TEST_F(TestMemory, writeRead_1word) {
    Memory mem(2);
    Word w = 0x1234;
    mem.write2(0, w);
    EXPECT_EQ(w, mem.read2(0));
}

TEST_F(TestMemory, writeRead_1dword) {
    Memory mem(4);
    Dword w = 0x12345678;
    mem.write4(0, w);
    EXPECT_EQ(w, mem.read4(0));
}

TEST_F(TestMemory, doNotWriteAnythingIfDataDoNotFit) {
    Memory mem1(1);
    mem1.write(0, 0xAB);

    ASSERT_EQ(0xAB, mem1.read(0));
    mem1.write2(0, 0x1234);
    EXPECT_EQ(0xAB, mem1.read(0));

    Memory mem2(2);
    mem2.write2(0, 0xABCD);

    ASSERT_EQ(0xABCD, mem2.read2(0));
    mem2.write4(0, 0x12345678);
    EXPECT_EQ(0xABCD, mem2.read2(0));
}