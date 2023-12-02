#include "CPU6502Register.hpp"
#include "Logger.hpp"

CPU6502Register::CPU6502Register(std::shared_ptr<PStatusReg6502>& pStatus,
                                 uint8_t reg)
    : pStatus_(pStatus)
    , reg_(reg)
{}

CPU6502Register& CPU6502Register::operator=(const uint8_t& val)
{
    DEBUG("Write to register 0x{:0>2X}\n", val);
    pStatus_->SetZeroFlag(val == 0);
    pStatus_->SetNegativeFlag((val & 0x80) != 0);
    reg_ = val;
    return *this;
}

CPU6502Register::operator uint8_t() const
{
    DEBUG("Read from register 0x{:0>2X}\n", reg_);
    return reg_;
}
