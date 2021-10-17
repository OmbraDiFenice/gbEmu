#include <Pch.h>

#include <gtest/gtest.h>
#include "../TestInstructions.h"

class Misc : public TestInstruction {};

#define SWAP(opcode, reg)               \
    TEST_F(Misc, SWAP_##opcode_##reg) { \
        setNextInstruction(opcode);     \
        cpu.reg = Byte{0xA1};           \
        cpu.setFlags("ZNHC");           \
        runAndCheck([](Cpu& cpu) {      \
            cpu.PC += 2;                \
            cpu.reg = Byte{0x1A};       \
            cpu.setFlags("znhc");       \
        });                             \
    }

SWAP(0xCB37, A);
SWAP(0xCB30, B);
SWAP(0xCB31, C);
SWAP(0xCB32, D);
SWAP(0xCB33, E);
SWAP(0xCB34, H);
SWAP(0xCB35, L);
TEST_F(Misc, SWAP__HL__) {
    setNextInstruction(0xCB36);
    cpu.HL             = Word{0xFF10};
    cpu.memory[0xFF10] = Byte{0x00};
    cpu.setFlags("zNHC");
    runAndCheck([](Cpu& cpu) {
        cpu.PC += 2;
        cpu.memory[0xFF10] = Byte{0x00};
        cpu.setFlags("Znhc");
    });
}
