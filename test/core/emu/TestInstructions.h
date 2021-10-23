#pragma once

#include <core/emu/Cpu.h>
#include <gtest/gtest.h>
#include "TestInstructions.h"

class TestInstruction : public testing::Test {
   protected:
    TestInstruction();
    void setNextInstruction(Word opcode);
    void setNextInstructions(const std::vector<Word>& opcodes);
    void setNextInstructions(const std::vector<std::pair<Word, Byte>>& opcodes);
    void setNextInstruction(Word opcode, Byte data);
    void setNextInstruction(Word opcode, Word data);
    void runAndCheck(std::function<void(Cpu&)>&& setupExpected);

   protected:
    void reset();

   protected:
    Cpu cpu;
};
