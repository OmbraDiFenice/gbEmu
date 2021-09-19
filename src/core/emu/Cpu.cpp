#include <Pch.h>

#include "Cpu.h"

std::unordered_map<Byte, Instruction> Cpu::_instructionSet;

void Cpu::Add(Byte iOpcode, Instruction&& iCode) {
    _instructionSet.emplace(iOpcode, iCode);
}

Cpu::Cpu(const Cpu& iCpu) {
    PC = iCpu.PC;
    BC = iCpu.BC;
    DE = iCpu.DE;
    HL = iCpu.HL;
    memcpy(memory, iCpu.memory, sizeof(Byte) * kMemSize);
}

void Cpu::tick() {
    Byte opcode              = memory[PC++];
    Instruction& instruction = _instructionSet.at(opcode);
    instruction(*this);
}

#include "Instructions.cpp"