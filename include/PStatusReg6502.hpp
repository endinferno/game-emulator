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
    typedef union
    {
        struct
        {
            uint8_t carryFlag_            : 1;
            uint8_t zeroFlag_             : 1;
            uint8_t interruptDisableFlag_ : 1;
            uint8_t decimalFlag_          : 1;
            uint8_t breakFlag_            : 1;
            uint8_t unusedFlag_           : 1;
            uint8_t overflowFlag_         : 1;
            uint8_t negativeFlag_         : 1;
        };
        uint8_t processorStatusReg_;
    } ProcessorStatusReg;

    ProcessorStatusReg pStatus_;
};
