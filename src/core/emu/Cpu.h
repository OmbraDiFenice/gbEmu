#pragma once

#include <core/emu/Memory.h>

class Cpu;

using Instruction = std::function<void(Cpu&)>;

class Cpu {
   public:
    static constexpr size_t kMemSize = 0x10000;
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
        TwoByteRegister(Word iVal) { *reinterpret_cast<::Word*>(this) = iVal; }
        bool operator==(const TwoByteRegister& iOther) const {
            return msb == iOther.msb && lsb == iOther.lsb;
        }
        operator Word() { return *reinterpret_cast<::Word*>(this); }
    };

    Word PC = 0;
    Byte memory[kMemSize];

    Byte A;

    TwoByteRegister BC;
    Byte& B = BC.msb;
    Byte& C = BC.lsb;

    TwoByteRegister DE;
    Byte& D = DE.msb;
    Byte& E = DE.lsb;

    TwoByteRegister HL;
    Byte& H = HL.msb;
    Byte& L = HL.lsb;

   public:
    Cpu() = default;
    Cpu(const Cpu& iCpu);
    void tick();

    static void Add(Byte iOpcode, Instruction&& iCode);

   private:
    static std::unordered_map<Byte, Instruction> _instructionSet;
};

template <Byte opcode>
struct InstructionInstantiator {
    InstructionInstantiator() { Cpu::Add(opcode, body); };
    static void body(Cpu& cpu){};
};

#define CPU_INSTRUCTION(opcode)                    \
    InstructionInstantiator<opcode> instr##opcode; \
    template <>                                    \
    void InstructionInstantiator<opcode>::body(Cpu& cpu)
