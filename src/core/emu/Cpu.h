#pragma once

#include <core/emu/Memory.h>

class Cpu;

using Instruction = std::function<void(Cpu&)>;

template <Word opcode>
struct InstructionInstantiator;

class Cpu {
   public:
    enum class Flag { Z = 7, N = 6, H = 5, C = 4 };
    struct TwoByteRegister {
        Byte msb;
        Byte lsb;
        TwoByteRegister() {
            msb = 0;
            lsb = 0;
        }
        TwoByteRegister(const TwoByteRegister& iOther) {
            msb = iOther.msb;
            lsb = iOther.lsb;
        }
        TwoByteRegister(Word iVal) { assign(iVal); }
        TwoByteRegister& operator=(Word iVal) {
            assign(iVal);
            return *this;
        }
        TwoByteRegister operator+=(Word iVal) {
            assign(Word{*this} + iVal);
            return *this;
        }
        TwoByteRegister operator-=(Word iVal) {
            assign(Word{*this} - iVal);
            return *this;
        }
        bool operator==(const TwoByteRegister& iOther) const {
            return msb == iOther.msb && lsb == iOther.lsb;
        }
        TwoByteRegister operator-(::Word iVal) {
            *this = Word{*this} - iVal;
            return *this;
        }
        TwoByteRegister& operator--() {
            assign(*this - Word{1});
            return *this;
        }
        TwoByteRegister& operator++() {
            assign(*this + Word{1});
            return *this;
        }
        TwoByteRegister operator++(int) {
            Word ret = *this;
            assign(*this + Word{1});
            return ret;
        }
        operator Word() { return msb << 8 | lsb; }

       private:
        void assign(Word iVal) {
            msb = getMsb(iVal);
            lsb = getLsb(iVal);
        }
        Byte getMsb(const Word& iWord) { return (iWord >> 8) & 0xFF; }
        Byte getLsb(const Word& iWord) { return iWord & 0xFF; }
    };

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
