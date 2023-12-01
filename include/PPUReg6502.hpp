#pragma once

#include <string>

#include <cstdint>

using std::uint8_t;

class PPUReg6502
{
public:
    PPUReg6502();

    uint8_t read(const uint16_t& addr) const;
    void write(const uint16_t& addr, const uint8_t val);

    std::string ToString() const;

    ~PPUReg6502() = default;

private:
    constexpr static uint16_t PPU_CTRL_REG_ADDR = 0x2000;
    constexpr static uint16_t PPU_MASK_REG_ADDR = 0x2001;
    constexpr static uint16_t PPU_STATUS_REG_ADDR = 0x2002;
    uint8_t ppuCtrl_ = 0;
    uint8_t ppuMask_ = 0;
    uint8_t ppuStatus_ = 0;
};
