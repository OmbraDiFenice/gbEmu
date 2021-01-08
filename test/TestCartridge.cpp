#include <gtest/gtest.h>
#include <core/emu/Cartridge.h>

class TestCartridge : public ::testing::Test {
   protected:
    Cartridge cart;

    TestCartridge() { cart.load("test/data/testcart.gbc"); }
};

TEST_F(TestCartridge, loadFailureOnFile) {
    Cartridge cart;
    EXPECT_FALSE(cart.load("nonexistent_file"));
}

TEST_F(TestCartridge, loadOk) {
    EXPECT_TRUE(cart.load("test/data/testcart.gbc"));
}

TEST_F(TestCartridge, getCartType) {
    EXPECT_EQ("ROM ONLY 13h MBC3+RAM+BATTERY", cart.getCartType());
}

TEST_F(TestCartridge, getRomSize) { EXPECT_EQ(32u, cart.getRomSize()); }

TEST_F(TestCartridge, getRamSize) { EXPECT_EQ(0u, cart.getRamSize()); }

TEST_F(TestCartridge, getDestination) {
    EXPECT_EQ("Japanese", cart.getDestination());
}

TEST_F(TestCartridge, getHeaderChecksum) {
    EXPECT_EQ(0x4F, cart.getHeaderChecksum());
    EXPECT_EQ(0x4F, cart.computeHeaderChecksum());
}