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

TEST_F(Misc, DAA) {
    std::map<Byte /* initial A */,
             std::pair<Byte /* result A */, bool /* expected C */>>
        table = {
            {0x00, {0x00, false}}, {0x01, {0x01, false}}, {0x02, {0x02, false}},
            {0x03, {0x03, false}}, {0x04, {0x04, false}}, {0x05, {0x05, false}},
            {0x06, {0x06, false}}, {0x07, {0x07, false}}, {0x08, {0x08, false}},
            {0x09, {0x09, false}}, {0x0A, {0x10, false}}, {0x0B, {0x11, false}},
            {0x0C, {0x12, false}}, {0x0D, {0x13, false}}, {0x0E, {0x14, false}},
            {0x0F, {0x15, false}}, {0x10, {0x16, false}}, {0x11, {0x17, false}},
            {0x12, {0x18, false}}, {0x13, {0x19, false}}, {0x14, {0x20, false}},
            {0x15, {0x21, false}}, {0x16, {0x22, false}}, {0x17, {0x23, false}},
            {0x18, {0x24, false}}, {0x19, {0x25, false}}, {0x1A, {0x26, false}},
            {0x1B, {0x27, false}}, {0x1C, {0x28, false}}, {0x1D, {0x29, false}},
            {0x1E, {0x30, false}}, {0x1F, {0x31, false}}, {0x20, {0x32, false}},
            {0x21, {0x33, false}}, {0x22, {0x34, false}}, {0x23, {0x35, false}},
            {0x24, {0x36, false}}, {0x25, {0x37, false}}, {0x26, {0x38, false}},
            {0x27, {0x39, false}}, {0x28, {0x40, false}}, {0x29, {0x41, false}},
            {0x2A, {0x42, false}}, {0x2B, {0x43, false}}, {0x2C, {0x44, false}},
            {0x2D, {0x45, false}}, {0x2E, {0x46, false}}, {0x2F, {0x47, false}},
            {0x30, {0x48, false}}, {0x31, {0x49, false}}, {0x32, {0x50, false}},
            {0x33, {0x51, false}}, {0x34, {0x52, false}}, {0x35, {0x53, false}},
            {0x36, {0x54, false}}, {0x37, {0x55, false}}, {0x38, {0x56, false}},
            {0x39, {0x57, false}}, {0x3A, {0x58, false}}, {0x3B, {0x59, false}},
            {0x3C, {0x60, false}}, {0x3D, {0x61, false}}, {0x3E, {0x62, false}},
            {0x3F, {0x63, false}}, {0x40, {0x64, false}}, {0x41, {0x65, false}},
            {0x42, {0x66, false}}, {0x43, {0x67, false}}, {0x44, {0x68, false}},
            {0x45, {0x69, false}}, {0x46, {0x70, false}}, {0x47, {0x71, false}},
            {0x48, {0x72, false}}, {0x49, {0x73, false}}, {0x4A, {0x74, false}},
            {0x4B, {0x75, false}}, {0x4C, {0x76, false}}, {0x4D, {0x77, false}},
            {0x4E, {0x78, false}}, {0x4F, {0x79, false}}, {0x50, {0x80, false}},
            {0x51, {0x81, false}}, {0x52, {0x82, false}}, {0x53, {0x83, false}},
            {0x54, {0x84, false}}, {0x55, {0x85, false}}, {0x56, {0x86, false}},
            {0x57, {0x87, false}}, {0x58, {0x88, false}}, {0x59, {0x89, false}},
            {0x5A, {0x90, false}}, {0x5B, {0x91, false}}, {0x5C, {0x92, false}},
            {0x5D, {0x93, false}}, {0x5E, {0x94, false}}, {0x5F, {0x95, false}},
            {0x60, {0x96, false}}, {0x61, {0x97, false}}, {0x62, {0x98, false}},
            {0x63, {0x99, false}}, {0x64, {0x00, true}},  {0x65, {0x01, true}},
            {0x66, {0x02, true}},  {0x67, {0x03, true}},  {0x68, {0x04, true}},
            {0x69, {0x05, true}},  {0x6A, {0x06, true}},  {0x6B, {0x07, true}},
            {0x6C, {0x08, true}},  {0x6D, {0x09, true}},  {0x6E, {0x10, true}},
            {0x6F, {0x11, true}},  {0x70, {0x12, true}},  {0x71, {0x13, true}},
            {0x72, {0x14, true}},  {0x73, {0x15, true}},  {0x74, {0x16, true}},
            {0x75, {0x17, true}},  {0x76, {0x18, true}},  {0x77, {0x19, true}},
            {0x78, {0x20, true}},  {0x79, {0x21, true}},  {0x7A, {0x22, true}},
            {0x7B, {0x23, true}},  {0x7C, {0x24, true}},  {0x7D, {0x25, true}},
            {0x7E, {0x26, true}},  {0x7F, {0x27, true}},  {0x80, {0x28, true}},
            {0x81, {0x29, true}},  {0x82, {0x30, true}},  {0x83, {0x31, true}},
            {0x84, {0x32, true}},  {0x85, {0x33, true}},  {0x86, {0x34, true}},
            {0x87, {0x35, true}},  {0x88, {0x36, true}},  {0x89, {0x37, true}},
            {0x8A, {0x38, true}},  {0x8B, {0x39, true}},  {0x8C, {0x40, true}},
            {0x8D, {0x41, true}},  {0x8E, {0x42, true}},  {0x8F, {0x43, true}},
            {0x90, {0x44, true}},  {0x91, {0x45, true}},  {0x92, {0x46, true}},
            {0x93, {0x47, true}},  {0x94, {0x48, true}},  {0x95, {0x49, true}},
            {0x96, {0x50, true}},  {0x97, {0x51, true}},  {0x98, {0x52, true}},
            {0x99, {0x53, true}},  {0x9A, {0x54, true}},  {0x9B, {0x55, true}},
            {0x9C, {0x56, true}},  {0x9D, {0x57, true}},  {0x9E, {0x58, true}},
            {0x9F, {0x59, true}},  {0xA0, {0x60, true}},  {0xA1, {0x61, true}},
            {0xA2, {0x62, true}},  {0xA3, {0x63, true}},  {0xA4, {0x64, true}},
            {0xA5, {0x65, true}},  {0xA6, {0x66, true}},  {0xA7, {0x67, true}},
            {0xA8, {0x68, true}},  {0xA9, {0x69, true}},  {0xAA, {0x70, true}},
            {0xAB, {0x71, true}},  {0xAC, {0x72, true}},  {0xAD, {0x73, true}},
            {0xAE, {0x74, true}},  {0xAF, {0x75, true}},  {0xB0, {0x76, true}},
            {0xB1, {0x77, true}},  {0xB2, {0x78, true}},  {0xB3, {0x79, true}},
            {0xB4, {0x80, true}},  {0xB5, {0x81, true}},  {0xB6, {0x82, true}},
            {0xB7, {0x83, true}},  {0xB8, {0x84, true}},  {0xB9, {0x85, true}},
            {0xBA, {0x86, true}},  {0xBB, {0x87, true}},  {0xBC, {0x88, true}},
            {0xBD, {0x89, true}},  {0xBE, {0x90, true}},  {0xBF, {0x91, true}},
            {0xC0, {0x92, true}},  {0xC1, {0x93, true}},  {0xC2, {0x94, true}},
            {0xC3, {0x95, true}},  {0xC4, {0x96, true}},  {0xC5, {0x97, true}},
            {0xC6, {0x98, true}},  {0xC7, {0x99, true}},  {0xC8, {0x00, true}},
            {0xC9, {0x01, true}},  {0xCA, {0x02, true}},  {0xCB, {0x03, true}},
            {0xCC, {0x04, true}},  {0xCD, {0x05, true}},  {0xCE, {0x06, true}},
            {0xCF, {0x07, true}},  {0xD0, {0x08, true}},  {0xD1, {0x09, true}},
            {0xD2, {0x10, true}},  {0xD3, {0x11, true}},  {0xD4, {0x12, true}},
            {0xD5, {0x13, true}},  {0xD6, {0x14, true}},  {0xD7, {0x15, true}},
            {0xD8, {0x16, true}},  {0xD9, {0x17, true}},  {0xDA, {0x18, true}},
            {0xDB, {0x19, true}},  {0xDC, {0x20, true}},  {0xDD, {0x21, true}},
            {0xDE, {0x22, true}},  {0xDF, {0x23, true}},  {0xE0, {0x24, true}},
            {0xE1, {0x25, true}},  {0xE2, {0x26, true}},  {0xE3, {0x27, true}},
            {0xE4, {0x28, true}},  {0xE5, {0x29, true}},  {0xE6, {0x30, true}},
            {0xE7, {0x31, true}},  {0xE8, {0x32, true}},  {0xE9, {0x33, true}},
            {0xEA, {0x34, true}},  {0xEB, {0x35, true}},  {0xEC, {0x36, true}},
            {0xED, {0x37, true}},  {0xEE, {0x38, true}},  {0xEF, {0x39, true}},
            {0xF0, {0x40, true}},  {0xF1, {0x41, true}},  {0xF2, {0x42, true}},
            {0xF3, {0x43, true}},  {0xF4, {0x44, true}},  {0xF5, {0x45, true}},
            {0xF6, {0x46, true}},  {0xF7, {0x47, true}},  {0xF8, {0x48, true}},
            {0xF9, {0x49, true}},  {0xFA, {0x50, true}},  {0xFB, {0x51, true}},
            {0xFC, {0x52, true}},  {0xFD, {0x53, true}},  {0xFE, {0x54, true}},
            {0xFF, {0x55, true}},
        };

    for (const auto& [initialA, expected] : table) {
        const auto& [expectedA, expectedC] = expected;
        reset();
        setNextInstruction(0x27);
        cpu.setFlag(Cpu::Flag::H, true);
        cpu.A = initialA;
        runAndCheck([&expectedA, &expectedC](Cpu& cpu) {
            cpu.PC += 1;
            cpu.A = expectedA;
            cpu.setFlag(Cpu::Flag::Z, cpu.A == 0);
            cpu.setFlag(Cpu::Flag::H, false);
            cpu.setFlag(Cpu::Flag::C, expectedC);
        });
    }
}

TEST_F(Misc, CPL) {
    setNextInstruction(0x2F);
    cpu.A = Byte{0x55};
    cpu.setFlags("nh");
    runAndCheck([](Cpu& cpu) {
        cpu.PC += 1;
        cpu.A = Byte{0xAA};
        cpu.setFlags("NH");
    });
}

TEST_F(Misc, CCF) {
    setNextInstruction(0x3F);
    cpu.setFlags("NHC");
    runAndCheck([](Cpu& cpu) {
        cpu.PC += 1;
        cpu.setFlags("nhc");
    });

    reset();

    setNextInstruction(0x3F);
    cpu.setFlags("NHc");
    runAndCheck([](Cpu& cpu) {
        cpu.PC += 1;
        cpu.setFlags("nhC");
    });
}

TEST_F(Misc, SCF) {
    setNextInstruction(0x37);
    cpu.setFlags("NHc");
    runAndCheck([](Cpu& cpu) {
        cpu.PC += 1;
        cpu.setFlags("nhC");
    });

    reset();

    setNextInstruction(0x37);
    cpu.setFlags("NHC");
    runAndCheck([](Cpu& cpu) {
        cpu.PC += 1;
        cpu.setFlags("nhC");
    });
}

TEST_F(Misc, NOP) {
    setNextInstruction(0x00);
    runAndCheck([](Cpu& cpu) { cpu.PC += 1; });
}