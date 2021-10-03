#pragma once

#include <core/emu/Cpu.h>
#include <gtest/gtest.h>
#include "TestInstructions.h"

class TestInstruction : public testing::Test {
   protected:
    TestInstruction();
    void setNextInstruction(Byte opcode);
    void setNextInstruction(Byte opcode, Byte data);
    void setNextInstruction(Byte opcode, Word data);
    void runAndCheck(std::function<void(Cpu&)>&& setupExpected);

   protected:
    Cpu cpu;
};
