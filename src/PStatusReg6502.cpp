#include "PStatusReg6502.hpp"
#include "fmt/core.h"

PStatusReg6502::PStatusReg6502()
    : pStatus_(0)
{}

bool PStatusReg6502::GetCarryFlag() const
{
    return GetBit(CARRY_FLAG_OFFSET);
}

void PStatusReg6502::SetCarryFlag(bool flag)
{
    SetBit(CARRY_FLAG_OFFSET, flag);
}

bool PStatusReg6502::GetZeroFlag() const
{
    return GetBit(ZERO_FLAG_OFFSET);
}

void PStatusReg6502::SetZeroFlag(bool flag)
{
    SetBit(ZERO_FLAG_OFFSET, flag);
}

bool PStatusReg6502::GetInterruptDisableFlag() const
{
    return GetBit(INTERRUPT_DISABLE_FLAG_OFFSET);
}

void PStatusReg6502::SetInterruptDisableFlag(bool flag)
{
    SetBit(INTERRUPT_DISABLE_FLAG_OFFSET, flag);
}

bool PStatusReg6502::GetDecimalModeFlag() const
{
    return GetBit(DECIMAL_FLAG_OFFSET);
}

void PStatusReg6502::SetDecimalModeFlag(bool flag)
{
    SetBit(DECIMAL_FLAG_OFFSET, flag);
}

bool PStatusReg6502::GetBreakFlag() const
{
    return GetBit(BREAK_FLAG_OFFSET);
}

void PStatusReg6502::SetBreakFlag(bool flag)
{
    SetBit(BREAK_FLAG_OFFSET, flag);
}

bool PStatusReg6502::GetOverflowFlag() const
{
    return GetBit(OVERFLOW_FLAG_OFFSET);
}

void PStatusReg6502::SetOverflowFlag(bool flag)
{
    SetBit(OVERFLOW_FLAG_OFFSET, flag);
}

bool PStatusReg6502::GetNegativeFlag() const
{
    return GetBit(NEGATIVE_FLAG_OFFSET);
}

void PStatusReg6502::SetNegativeFlag(bool flag)
{
    SetBit(NEGATIVE_FLAG_OFFSET, flag);
}

bool PStatusReg6502::GetBit(const uint8_t offset) const
{
    uint8_t mask = 0x01 << offset;
    return (pStatus_ & mask) != 0;
}

void PStatusReg6502::SetBit(const uint8_t offset, bool flag)
{
    uint8_t base = 0x01;
    if (flag) {
        pStatus_ |= (base << offset);
    }
    else {
        pStatus_ &= ~(base << offset);
    }
}

std::string PStatusReg6502::ToString() const
{
    std::string fmtStr;
    fmtStr += fmt::format("Processor Status Register\n");
    fmtStr += fmt::format("{:<8} {:<8} {:<8} {:<8} {:<8} {:<8} {:<8}\n",
                          "Carry",
                          "Zero",
                          "IntDis",
                          "DemMode",
                          "Break",
                          "Overflow",
                          "Negative");
    fmtStr += fmt::format("{:<8} {:<8} {:<8} {:<8} {:<8} {:<8} {:<8}\n",
                          GetCarryFlag(),
                          GetZeroFlag(),
                          GetInterruptDisableFlag(),
                          GetDecimalModeFlag(),
                          GetBreakFlag(),
                          GetOverflowFlag(),
                          GetNegativeFlag());
    return fmtStr;
}
