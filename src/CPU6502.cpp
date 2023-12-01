#include "CPU6502.hpp"
#include "NesOpcode.hpp"
#include "fmt/core.h"

CPU6502::CPU6502(std::shared_ptr<NesReader>& nesReader)
    : pStatus_(std::make_shared<PStatusReg6502>())
    , memory_(std::make_shared<Memory6502>(nesReader))
{}

Opcode6502 CPU6502::DecodeOpcode(uint8_t& opcode) const
{
    switch (opcode) {
    case 0x10: return Opcode6502::BPL;
    case 0x78: return Opcode6502::SEI;
    case 0x8D: return Opcode6502::STA;
    case 0x9A: return Opcode6502::TXS;
    case 0xA0: return Opcode6502::LDY;
    case 0xA2: return Opcode6502::LDX;
    case 0xAD:
    case 0xA9:
    case 0xBD: return Opcode6502::LDA;
    case 0xB0: return Opcode6502::BCS;
    case 0xC9: return Opcode6502::CMP;
    case 0xD8: return Opcode6502::CLD;
    default: return Opcode6502::INVALID;
    }
}

void CPU6502::InputOpcode(const Opcode6502& opcode)
{
    switch (opcode) {
    case Opcode6502::SEI:
    {
        pStatus_->SetInterruptDisableFlag(true);
        break;
    }
    case Opcode6502::CLD:
    {
        pStatus_->SetDecimalModeFlag(false);
        break;
    }
    case Opcode6502::TXS:
    {
        sp_ = xReg_;
        break;
    }
    case Opcode6502::BPL:
    {
        if (!pStatus_->GetNegativeFlag()) {
            uint8_t pcOffset = memory_->read(pc_);
            pc_++;
            pc_ = static_cast<uint16_t>(pc_ + pcOffset);
        }
        else {
            pc_++;
        }
        break;
    }
    case Opcode6502::BCS:
    {
        if (pStatus_->GetCarryFlag()) {
            uint8_t pcOffset = memory_->read(pc_);
            pc_++;
            pc_ = static_cast<uint16_t>(pc_ + pcOffset);
        }
        else {
            pc_++;
        }
        break;
    }
    default: break;
    }
}

void CPU6502::InputOpcode(const Opcode6502& opcode, uint8_t& val)
{
    switch (opcode) {
    case Opcode6502::LDA:
    {
        StoreDataAccumReg(val);
        break;
    }
    case Opcode6502::LDX:
    {
        StoreDataXReg(val);
        break;
    }
    case Opcode6502::LDY:
    {
        StoreDataYReg(val);
        break;
    }
    case Opcode6502::CMP:
    {
        pStatus_->SetCarryFlag(accumReg_ >= val);
        pStatus_->SetZeroFlag(accumReg_ == val);
        pStatus_->SetNegativeFlag((val & 0x80) != 0);
        break;
    }
    default:
    {
        break;
    }
    }
}

void CPU6502::InputOpcode(const Opcode6502& opcode, uint16_t& val)
{
    switch (opcode) {
    case Opcode6502::STA:
    {
        memory_->write(val, accumReg_);
        break;
    }
    case Opcode6502::LDA:
    {
        StoreDataAccumReg(memory_->read(val));
        break;
    }
    default:
    {
        break;
    }
    }
}

void CPU6502::StoreDataAccumReg(uint8_t val)
{
    pStatus_->SetZeroFlag(val == 0);
    pStatus_->SetNegativeFlag((val & 0x80) != 0);
    accumReg_ = val;
}

void CPU6502::StoreDataXReg(uint8_t val)
{
    pStatus_->SetZeroFlag(val == 0);
    pStatus_->SetNegativeFlag((val & 0x80) != 0);
    xReg_ = val;
}

void CPU6502::StoreDataYReg(uint8_t val)
{
    pStatus_->SetZeroFlag(val == 0);
    pStatus_->SetNegativeFlag((val & 0x80) != 0);
    yReg_ = val;
}

std::string CPU6502::ToString() const
{
    std::string fmtStr;
    fmtStr += fmt::format("CPU Registers\n");
    fmtStr += fmt::format(
        "{:<8}{:<8}{:<8}{:<8}{:<8}\n", "PC", "SP", "Accum", "X", "Y");
    fmtStr += fmt::format(
        "{:<8X}{:<8X}{:<8X}{:<8X}{:<8X}\n", pc_, sp_, accumReg_, xReg_, yReg_);
    fmtStr += pStatus_->ToString();
    fmtStr += fmt::format("\n");
    fmtStr += memory_->ToString();
    return fmtStr;
}
