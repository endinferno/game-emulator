#include "NesOpcode.hpp"
#include "Logger.hpp"

void PrintOpcode(const Opcode6502& opcode)
{
    switch (opcode) {
    case Opcode6502::SEI: DEBUG("SEI"); break;
    case Opcode6502::CLD: DEBUG("CLD"); break;
    case Opcode6502::LDA: DEBUG("LDA"); break;
    case Opcode6502::STA: DEBUG("STA"); break;
    case Opcode6502::LDX: DEBUG("LDX"); break;
    case Opcode6502::TXS: DEBUG("TXS"); break;
    case Opcode6502::BPL: DEBUG("BPL"); break;
    case Opcode6502::LDY: DEBUG("LDY"); break;
    case Opcode6502::CMP: DEBUG("CMP"); break;
    case Opcode6502::BCS: DEBUG("BCS"); break;
    case Opcode6502::INVALID: DEBUG("INVALID"); break;
    default: break;
    }
}
