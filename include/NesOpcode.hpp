#pragma once

#include <cstdint>

using std::uint8_t;

enum class Opcode6502 : uint8_t
{
    SEI,
    CLD,
    LDA,
    STA,
    LDX,
    TXS,
    BPL,
    LDY,
    CMP,
    BCS,
    INVALID,
};

void PrintOpcode(const Opcode6502& opcode);
