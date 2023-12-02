#pragma once

#include <vector>

#include <cstdint>

#include "MapperNROM.hpp"
#include "PPU6502.hpp"

using std::uint8_t;
using std::uint16_t;

class Memory6502
{
public:
    explicit Memory6502(std::shared_ptr<NesReader>& nesReader);

    void Reset();
    uint8_t Read(const uint16_t& addr) const;
    uint8_t ReadByte(const uint16_t& addr) const;
    uint16_t ReadWord(const uint16_t& addr) const;
    void Write(const uint16_t& addr, const uint8_t val);
    std::string ToString() const;

    ~Memory6502() = default;

private:
    constexpr static uint16_t INTERNAL_RAM_RANGE = 0x2000;
    constexpr static uint16_t INTERNAL_RAM_SIZE = 0x0800;
    constexpr static uint16_t PPU_REG_RANGE = 0x4000;
    constexpr static uint16_t PPU_REG_SIZE = 0x0008;
    constexpr static uint16_t APU_REG_RANGE = 0x4020;
    constexpr static uint16_t APU_REG_SIZE = 0x0018;
    constexpr static uint16_t PRG_RAM_RANGE = 0x8000;
    std::vector<uint8_t> internalRam_;
    PPU6502 ppuReg_;
    MapperNROM mapper_;
};
