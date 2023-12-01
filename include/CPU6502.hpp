#pragma once

#include <cstdint>

#include "Memory6502.hpp"
#include "NesOpcode.hpp"
#include "NesReader.hpp"
#include "PStatusReg6502.hpp"

using std::uint8_t;

class CPU6502
{
public:
    CPU6502(std::shared_ptr<NesReader>& nesReader);

    Opcode6502 DecodeOpcode(uint8_t& opcode) const;
    void InputOpcode(const Opcode6502& opcode);
    void InputOpcode(const Opcode6502& opcode, uint8_t& val);
    void InputOpcode(const Opcode6502& opcode, uint16_t& val);

    std::string ToString() const;

    ~CPU6502() = default;

private:
    void StoreDataAccumReg(uint8_t val);
    void StoreDataXReg(uint8_t val);
    void StoreDataYReg(uint8_t val);

private:
    std::shared_ptr<PStatusReg6502> pStatus_;
    std::shared_ptr<Memory6502> memory_;
    uint16_t pc_ = 0;
    uint8_t sp_ = 0;
    uint8_t accumReg_ = 0;
    uint8_t xReg_ = 0;
    uint8_t yReg_ = 0;
};
