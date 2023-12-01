#pragma once

#include <cstdint>

#include "Memory6502.hpp"
#include "NesOpcode.hpp"
#include "NesReader.hpp"
#include "PStatusReg6502.hpp"

using std::uint8_t;
using std::uint16_t;

class CPU6502
{
public:
    CPU6502(std::shared_ptr<NesReader>& nesReader);

    void Reset();
    Opcode6502 ReadOpcode();
    void InputOpcode(const Opcode6502& opcode);
    void InputOpcode(const Opcode6502& opcode, uint8_t val);
    void InputOpcode(const Opcode6502& opcode, uint16_t val);

    std::string ToString() const;

    ~CPU6502() = default;

private:
    Opcode6502 DecodeOpcode(uint8_t opcode) const;
    void IncreasePC(uint16_t offset);
    void Reset(uint16_t startAddr);
    void StoreDataAccumReg(uint8_t val);
    void StoreDataXReg(uint8_t val);
    void StoreDataYReg(uint8_t val);

private:
    constexpr static uint16_t RESET_VECTOR_ADDR = 0xFFFC;

    std::shared_ptr<PStatusReg6502> pStatus_;
    std::shared_ptr<Memory6502> memory_;
    uint16_t pc_ = 0;
    uint8_t sp_ = 0;
    uint8_t accumReg_ = 0;
    uint8_t xReg_ = 0;
    uint8_t yReg_ = 0;
};
