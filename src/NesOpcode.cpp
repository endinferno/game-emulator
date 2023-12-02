#include "NesOpcode.hpp"
#include "Logger.hpp"

void PrintOpcode(const Opcode6502& opcode)
{
    switch (opcode) {
    case Opcode6502::SEI: DEBUG("SEI\n"); break;
    case Opcode6502::CLD: DEBUG("CLD\n"); break;
    case Opcode6502::TXS: DEBUG("TXS\n"); break;
    case Opcode6502::BPL: DEBUG("BPL\n"); break;
    case Opcode6502::LDAImmediate: DEBUG("LDA Immediate\n"); break;
    case Opcode6502::LDAAbsolute: DEBUG("LDA Absolute\n"); break;
    case Opcode6502::LDXImmediate: DEBUG("LDX Immediate\n"); break;
    case Opcode6502::STAAbsolute: DEBUG("STA Absolute\n"); break;
    case Opcode6502::INVALID: DEBUG("INVALID\n"); break;
    default: break;
    }
}
