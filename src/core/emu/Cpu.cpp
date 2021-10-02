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
    A  = iCpu.A;
    memcpy(memory, iCpu.memory, sizeof(Byte) * kMemSize);
}

void Cpu::tick() {
    Byte opcode = memory[PC++];
    if (_instructionSet.count(opcode) != 0) {
        Instruction& instruction = _instructionSet.at(opcode);
        instruction(*this);
    }
}

#include "Instructions.cpp"