#pragma once

#include <memory>

#include <cstdint>

#include "PStatusReg6502.hpp"

using std::uint8_t;

class CPU6502Register
{
public:
    explicit CPU6502Register(std::shared_ptr<PStatusReg6502>& pStatus,
                             uint8_t reg);

    CPU6502Register& operator=(const uint8_t& val);
    operator uint8_t() const;

    ~CPU6502Register() = default;

private:
    std::shared_ptr<PStatusReg6502> pStatus_;
    uint8_t reg_;
};
