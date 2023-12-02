#pragma once

#include <cstdint>

using std::uint8_t;

enum class Opcode6502 : uint8_t
{
    SEI = 0x78,
    CLD = 0xD8,
    TXS = 0x9A,
    BPL = 0x10,
    LDAImmediate = 0xA9,
    LDAAbsolute = 0xAD,
    LDXImmediate = 0xA2,
    STAAbsolute = 0x8D,
    INVALID,
};

void PrintOpcode(const Opcode6502& opcode);
