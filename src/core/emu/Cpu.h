#pragma once

#include <core/emu/Memory.h>

class Cpu;

using Instruction = std::function<void(Cpu&)>;

struct TwoByteRegister {
    Byte msb;
    Byte lsb;
    TwoByteRegister();
    TwoByteRegister(const TwoByteRegister& iOther);
    TwoByteRegister(Word iVal);
    TwoByteRegister& operator=(Word iVal);
    TwoByteRegister operator+=(Word iVal);
    TwoByteRegister operator-=(Word iVal);
    bool operator==(const TwoByteRegister& iOther) const;
    TwoByteRegister operator-(::Word iVal);
    TwoByteRegister& operator--();
    TwoByteRegister& operator++();
    TwoByteRegister operator++(int);
    operator Word();

   private:
    void assign(Word iVal);
    Byte getMsb(const Word& iWord);
    Byte getLsb(const Word& iWord);
};

class Cpu {
   public:
    enum class Flag { Z = 7, N = 6, H = 5, C = 4 };
    Word PC            = 0;
    TwoByteRegister SP = 0;
    Memory memory;

    TwoByteRegister AF;
    Byte& A = AF.msb;
    Byte& F = AF.lsb;

    TwoByteRegister BC;
    Byte& B = BC.msb;
    Byte& C = BC.lsb;

    TwoByteRegister DE;
    Byte& D = DE.msb;
    Byte& E = DE.lsb;

    TwoByteRegister HL;
    Byte& H = HL.msb;
    Byte& L = HL.lsb;

    bool interruptsEnabled = true;

   public:
    Cpu() = default;
    Cpu(const Cpu& iCpu);
    void tick();

    bool getFlag(Flag iFlag) const;
    void setFlag(Cpu::Flag iFlag, bool iValue);
    void setFlags(const std::string& iFlags);

    void setDisableInterruptAfterNextInstruction();
    void setEnableInterruptAfterNextInstruction();

    Word readWordImmediateInstructionValue();

    static void Add(Word iOpcode, Instruction&& iCode, bool iExtInstruction);

    Byte sum(Byte iVal1, Byte iVal2, bool withCarry = false);
    Word sum(Word iVal1, Word iVal2);
    Byte sub(Byte iVal1, Byte iVal2, bool withCarry = false);
    Word sub(Word iVal1, Word iVal2);

   private:
    std::tuple<Byte, bool, bool> adder(Byte iVal1, Byte iVal2, bool c0) const;

   private:
    static std::unordered_map<Word, Instruction> _instructionSet;
    int _shouldDisableInterrupt = -1;
    int _shouldEnableInterrupt  = -1;
};

template <Word opcode>
struct InstructionInstantiator {
    InstructionInstantiator() { Cpu::Add(opcode, body, false); };
    static void body(Cpu& cpu){};
};

#define CPU_INSTRUCTION(opcode)                    \
    InstructionInstantiator<opcode> instr##opcode; \
    template <>                                    \
    void InstructionInstantiator<opcode>::body(Cpu& cpu)
