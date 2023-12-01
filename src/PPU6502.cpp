#include "PPU6502.hpp"
#include "fmt/core.h"

PPU6502::PPU6502() {}

void PPU6502::Reset()
{
    ppuCtrl_ = 0;
    ppuMask_ = 0;
    ppuStatus_ = 0;
}

uint8_t PPU6502::Read(const uint16_t& addr) const
{
    switch (addr) {
    case PPU_STATUS_REG_ADDR:
    {
        return ppuStatus_;
    }
    default:
    {
        break;
    }
    }
    return 0;
}

void PPU6502::Write(const uint16_t& addr, const uint8_t val)
{
    switch (addr) {
    case PPU_CTRL_REG_ADDR:
    {
        ppuCtrl_ = val;
        break;
    }
    case PPU_MASK_REG_ADDR:
    {
        break;
    }
    default:
    {
        break;
    }
    }
    return;
}

std::string PPU6502::ToString() const
{
    std::string fmtStr;
    fmtStr += fmt::format("PPU Registers\n");
    fmtStr += fmt::format("{:<9}{:<9}{:<9}\n", "Ctrl", "Mask", "Status");
    fmtStr += fmt::format(
        "{:0>8B} {:0>8B} {:0>8B}\n", ppuCtrl_, ppuMask_, ppuStatus_);
    return fmtStr;
}
