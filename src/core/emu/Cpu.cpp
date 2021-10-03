#include <Pch.h>

#include "Cpu.h"

std::unordered_map<Byte, Instruction> Cpu::_instructionSet;

void Cpu::Add(Byte iOpcode, Instruction&& iCode) {
    ASSERT(_instructionSet.count(iOpcode) == 0, "cannot insert instruction " +
                                                    std::to_string(iOpcode) +
                                                    ", opcode already taken");
    _instructionSet.emplace(iOpcode, iCode);
}

Cpu::Cpu(const Cpu& iCpu) {
    PC = iCpu.PC;
    SP = iCpu.SP;
    BC = iCpu.BC;
    DE = iCpu.DE;
    HL = iCpu.HL;
    AF = iCpu.AF;
    memcpy(memory, iCpu.memory, sizeof(Byte) * kMemSize);
}

void Cpu::tick() {
    Byte opcode = memory[PC++];
    if (_instructionSet.count(opcode) != 0) {
        Instruction& instruction = _instructionSet.at(opcode);
        instruction(*this);
    }
}

bool Cpu::getFlag(Cpu::Flag iFlag) const {
    Byte mask = 0x01 << static_cast<int>(
                    iFlag);  // rely on correct enum integer mapping
    return (F & mask) != 0;
}

void Cpu::setFlag(Cpu::Flag iFlag, bool iValue) {
    Byte mask = 0x01 << static_cast<int>(
                    iFlag);  // rely on correct enum integer mapping
    if (iValue) {
        F |= mask;
    } else {
        F &= ~mask;
    }
}

void Cpu::setFlags(const std::string& iFlags) {
    for (char f : iFlags) {
        switch (f) {
            case 'z':
                setFlag(Cpu::Flag::Z, false);
                break;
            case 'n':
                setFlag(Cpu::Flag::N, false);
                break;
            case 'h':
                setFlag(Cpu::Flag::H, false);
                break;
            case 'c':
                setFlag(Cpu::Flag::C, false);
                break;
            case 'Z':
                setFlag(Cpu::Flag::Z, true);
                break;
            case 'N':
                setFlag(Cpu::Flag::N, true);
                break;
            case 'H':
                setFlag(Cpu::Flag::H, true);
                break;
            case 'C':
                setFlag(Cpu::Flag::C, true);
                break;
        }
    }
}

Byte Cpu::sum(Byte iVal1, Byte iVal2) {
    auto [result, halfCarry, fullCarry] = adder(iVal1, iVal2, false);
    setFlag(Flag::H, halfCarry);
    setFlag(Flag::C, fullCarry);
    return result;
}

Byte Cpu::sub(Byte iVal1, Byte iVal2) {
    auto [result, halfCarry, fullCarry] = adder(iVal1, ~iVal2, true);
    setFlag(Flag::H, !halfCarry);
    setFlag(Flag::C, !fullCarry);
    return result;
}

Word Cpu::sum(Word iVal1, Word iVal2) {
    auto [lsbResult, lsbHalfCarry, lsbFullCarry] =
        adder(iVal1 & 0xFF, iVal2 & 0xFF, false);
    auto [msbResult, msbHalfCarry, msbFullCarry] =
        adder(iVal1 >> 8, iVal2 >> 8, lsbFullCarry);
    setFlag(Flag::H, msbHalfCarry);
    setFlag(Flag::C, msbFullCarry);
    return (msbResult << 8) | (lsbResult & 0xFF);
}

Word Cpu::sub(Word iVal1, Word iVal2) {
    auto [lsbResult, lsbHalfCarry, lsbFullCarry] =
        adder(iVal1 & 0xFF, ~(iVal2 & 0xFF), true);
    auto [msbResult, msbHalfCarry, msbFullCarry] =
        adder(iVal1 >> 8, ~(iVal2 >> 8), lsbFullCarry);
    setFlag(Flag::H, !msbHalfCarry);
    setFlag(Flag::C, !msbFullCarry);
    return (msbResult << 8) | (lsbResult & 0xFF);
}

std::tuple<Byte, bool, bool> Cpu::adder(Byte iVal1, Byte iVal2, bool c0) const {
    uint32_t halfResult = (iVal1 & 0x0F) + (iVal2 & 0x0F) + c0;
    bool halfCarry      = halfResult & 0xFFFFFFF0;
    uint32_t fullResult = (iVal1 >> 4) + (iVal2 >> 4) + halfCarry;
    bool fullCarry      = fullResult & 0xFFFFFFF0;
    Byte result         = (fullResult << 4) | (halfResult & 0x0F);
    return std::make_tuple(result, halfCarry, fullCarry);
}

#include "Instructions.cpp"