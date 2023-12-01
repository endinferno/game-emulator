#include "NesOpcode.hpp"
#include "Logger.hpp"

void PrintOpcode(const Opcode6502& opcode)
{
    switch (opcode) {
    case Opcode6502::SEI: DEBUG("SEI\n"); break;
    case Opcode6502::CLD: DEBUG("CLD\n"); break;
    case Opcode6502::LDA: DEBUG("LDA\n"); break;
    case Opcode6502::STA: DEBUG("STA\n"); break;
    case Opcode6502::LDX: DEBUG("LDX\n"); break;
    case Opcode6502::TXS: DEBUG("TXS\n"); break;
    case Opcode6502::BPL: DEBUG("BPL\n"); break;
    case Opcode6502::LDY: DEBUG("LDY\n"); break;
    case Opcode6502::CMP: DEBUG("CMP\n"); break;
    case Opcode6502::BCS: DEBUG("BCS\n"); break;
    case Opcode6502::INVALID: DEBUG("INVALID\n"); break;
    default: break;
    }
}
