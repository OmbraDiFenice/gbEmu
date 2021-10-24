#include <Pch.h>

#include "Cpu.h"

// ---------------------- TwoByteRegister ----------------------

TwoByteRegister::TwoByteRegister() {
    msb = 0;
    lsb = 0;
}

TwoByteRegister::TwoByteRegister(const TwoByteRegister& iOther) {
    msb = iOther.msb;
    lsb = iOther.lsb;
}

TwoByteRegister::TwoByteRegister(Word iVal) { assign(iVal); }

TwoByteRegister& TwoByteRegister::operator=(Word iVal) {
    assign(iVal);
    return *this;
}

TwoByteRegister TwoByteRegister::operator+=(Word iVal) {
    assign(Word{*this} + iVal);
    return *this;
}

TwoByteRegister TwoByteRegister::operator-=(Word iVal) {
    assign(Word{*this} - iVal);
    return *this;
}

bool TwoByteRegister::operator==(const TwoByteRegister& iOther) const {
    return msb == iOther.msb && lsb == iOther.lsb;
}

TwoByteRegister TwoByteRegister::operator-(::Word iVal) {
    *this = Word{*this} - iVal;
    return *this;
}

TwoByteRegister& TwoByteRegister::operator--() {
    assign(*this - Word{1});
    return *this;
}

TwoByteRegister& TwoByteRegister::operator++() {
    assign(*this + Word{1});
    return *this;
}

TwoByteRegister TwoByteRegister::operator++(int) {
    Word ret = *this;
    assign(*this + Word{1});
    return ret;
}

TwoByteRegister::operator Word() { return msb << 8 | lsb; }

void TwoByteRegister::assign(Word iVal) {
    msb = getMsb(iVal);
    lsb = getLsb(iVal);
}

Byte TwoByteRegister::getMsb(const Word& iWord) { return (iWord >> 8) & 0xFF; }

Byte TwoByteRegister::getLsb(const Word& iWord) { return iWord & 0xFF; }

// ---------------------------- Cpu ----------------------------

std::unordered_map<Word, Instruction> Cpu::_instructionSet;

void Cpu::Add(Word iOpcode, Instruction&& iCode, bool iExtInstruction) {
    ASSERT(_instructionSet.count(iOpcode) == 0, "cannot insert instruction " +
                                                    std::to_string(iOpcode) +
                                                    ", opcode already taken");
    _instructionSet.emplace(iOpcode, iCode);
}

Cpu::Cpu(const Cpu& iCpu) : memory(iCpu.memory) {
    PC                = iCpu.PC;
    SP                = iCpu.SP;
    BC                = iCpu.BC;
    DE                = iCpu.DE;
    HL                = iCpu.HL;
    AF                = iCpu.AF;
    interruptsEnabled = iCpu.interruptsEnabled;
}

void Cpu::tick() {
    Word opcode = memory[PC++];
    if (opcode == 0xCB) {
        opcode = (opcode << 8) + memory[PC++];
    }
    if (_instructionSet.count(opcode) != 0) {
        Instruction& instruction = _instructionSet.at(opcode);
        instruction(*this);
    } else {
        LOG_WARN("unknown opcode " << opcode << ", skipping");
    }

    if (_shouldDisableInterrupt > 0 && --_shouldDisableInterrupt == 0) {
        interruptsEnabled = false;
    }

    if (_shouldEnableInterrupt > 0 && --_shouldEnableInterrupt == 0) {
        interruptsEnabled = true;
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

void Cpu::setDisableInterruptAfterNextInstruction() {
    _shouldDisableInterrupt = 2;
}

void Cpu::setEnableInterruptAfterNextInstruction() {
    _shouldEnableInterrupt = 2;
}

Word Cpu::readWordImmediateInstructionValue() {
    Byte immediateLsb = memory[PC++];
    Byte immediateMsb = memory[PC++];
    return (immediateMsb << 8) | immediateLsb;
}

Byte Cpu::sum(Byte iVal1, Byte iVal2, bool withCarry) {
    auto [result, halfCarry, fullCarry] =
        adder(iVal1, iVal2, withCarry && getFlag(Flag::C));
    setFlag(Flag::H, halfCarry);
    setFlag(Flag::C, fullCarry);
    return result;
}

Byte Cpu::sub(Byte iVal1, Byte iVal2, bool withCarry) {
    auto [result, halfCarry, fullCarry] =
        adder(iVal1, ~iVal2, !(withCarry && getFlag(Flag::C)));
    setFlag(Flag::H, halfCarry);
    setFlag(Flag::C, fullCarry);
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