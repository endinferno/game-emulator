#pragma once

#include <string>

#include <cstdint>

using std::uint8_t;

class PStatusReg6502
{
public:
    PStatusReg6502();

    void Reset();
    bool GetCarryFlag() const;
    void SetCarryFlag(bool flag);

    bool GetZeroFlag() const;
    void SetZeroFlag(bool flag);

    bool GetInterruptDisableFlag() const;
    void SetInterruptDisableFlag(bool flag);

    bool GetDecimalModeFlag() const;
    void SetDecimalModeFlag(bool flag);

    bool GetBreakFlag() const;
    void SetBreakFlag(bool flag);

    bool GetOverflowFlag() const;
    void SetOverflowFlag(bool flag);

    bool GetNegativeFlag() const;
    void SetNegativeFlag(bool flag);

    std::string ToString() const;

    ~PStatusReg6502() = default;

private:
    bool GetBit(const uint8_t offset) const;
    void SetBit(const uint8_t offset, bool flag);

private:
    constexpr static uint8_t CARRY_FLAG_OFFSET = 0x00;
    constexpr static uint8_t ZERO_FLAG_OFFSET = 0x01;
    constexpr static uint8_t INTERRUPT_DISABLE_FLAG_OFFSET = 0x02;
    constexpr static uint8_t DECIMAL_FLAG_OFFSET = 0x03;
    constexpr static uint8_t BREAK_FLAG_OFFSET = 0x04;
    constexpr static uint8_t OVERFLOW_FLAG_OFFSET = 0x06;
    constexpr static uint8_t NEGATIVE_FLAG_OFFSET = 0x07;
    uint8_t pStatus_;
};
