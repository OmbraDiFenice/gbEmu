#include <Pch.h>

#include <core/emu/Memory.h>
#include <gtest/gtest.h>

class TestMemory : public ::testing::Test {};

TEST_F(TestMemory, writeRead_byte) {
    Memory mem;
    Byte b = 0xAB;
    mem[0] = b;
    EXPECT_EQ(b, mem[0]);
}
