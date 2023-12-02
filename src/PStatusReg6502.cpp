#include "PStatusReg6502.hpp"
#include "fmt/core.h"

PStatusReg6502::PStatusReg6502()
    : pStatus_({ .processorStatusReg_ = 0 })
{}

void PStatusReg6502::Reset()
{
    pStatus_.processorStatusReg_ = 0;
}

bool PStatusReg6502::GetCarryFlag() const
{
    return !!pStatus_.carryFlag_;
}

void PStatusReg6502::SetCarryFlag(bool flag)
{
    pStatus_.carryFlag_ = flag;
}

bool PStatusReg6502::GetZeroFlag() const
{
    return !!pStatus_.zeroFlag_;
}

void PStatusReg6502::SetZeroFlag(bool flag)
{
    pStatus_.zeroFlag_ = flag;
}

bool PStatusReg6502::GetInterruptDisableFlag() const
{
    return !!pStatus_.interruptDisableFlag_;
}

void PStatusReg6502::SetInterruptDisableFlag(bool flag)
{
    pStatus_.interruptDisableFlag_ = flag;
}

bool PStatusReg6502::GetDecimalModeFlag() const
{
    return !!pStatus_.decimalFlag_;
}

void PStatusReg6502::SetDecimalModeFlag(bool flag)
{
    pStatus_.decimalFlag_ = flag;
}

bool PStatusReg6502::GetBreakFlag() const
{
    return !!pStatus_.breakFlag_;
}

void PStatusReg6502::SetBreakFlag(bool flag)
{
    pStatus_.breakFlag_ = flag;
}

bool PStatusReg6502::GetOverflowFlag() const
{
    return !!pStatus_.overflowFlag_;
}

void PStatusReg6502::SetOverflowFlag(bool flag)
{
    pStatus_.overflowFlag_ = flag;
}

bool PStatusReg6502::GetNegativeFlag() const
{
    return !!pStatus_.negativeFlag_;
}

void PStatusReg6502::SetNegativeFlag(bool flag)
{
    pStatus_.negativeFlag_ = flag;
}

std::string PStatusReg6502::ToString() const
{
    std::string fmtStr;
    fmtStr += fmt::format("Processor Status Register\n");
    fmtStr += fmt::format("{:<9}{:<9}{:<9}{:<9}{:<9}{:<9}{:<9}\n",
                          "Carry",
                          "Zero",
                          "IntDis",
                          "DemMode",
                          "Break",
                          "Overflow",
                          "Negative");
    fmtStr += fmt::format("{:<9}{:<9}{:<9}{:<9}{:<9}{:<9}{:<9}",
                          GetCarryFlag(),
                          GetZeroFlag(),
                          GetInterruptDisableFlag(),
                          GetDecimalModeFlag(),
                          GetBreakFlag(),
                          GetOverflowFlag(),
                          GetNegativeFlag());
    return fmtStr;
}
