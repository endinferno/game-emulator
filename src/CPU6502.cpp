#include <cassert>

#include "CPU6502.hpp"
#include "Logger.hpp"
#include "NesOpcode.hpp"

CPU6502::CPU6502(std::shared_ptr<NesReader>& nesReader)
    : pStatus_(std::make_shared<PStatusReg6502>())
    , memory_(std::make_shared<Memory6502>(nesReader))
{
    Reset();
}

void CPU6502::Reset()
{
    uint16_t lowerAddr = memory_->Read(RESET_VECTOR_ADDR);
    uint16_t upperAddr = memory_->Read(RESET_VECTOR_ADDR + 1);
    Reset(lowerAddr | upperAddr << 8);
}

void CPU6502::Reset(uint16_t startAddr)
{
    DEBUG("Reset CPU at address: {:X}", startAddr);
    accumReg_ = 0;
    xReg_ = 0;
    yReg_ = 0;
    sp_ = 0xFD;
    pc_ = startAddr;
    pStatus_->Reset();
    memory_->Reset();
}

void CPU6502::Run()
{
    int cnt = 5;
    while (cnt--) {
        auto opcode = ReadOpcode();
        PrintOpcode(opcode);
        InputOpcode(opcode);
    }
}

Opcode6502 CPU6502::ReadOpcode()
{
    uint8_t opcodeVal = memory_->Read(pc_);
    IncreasePC(1);
    return DecodeOpcode(opcodeVal);
}

Opcode6502 CPU6502::DecodeOpcode(uint8_t opcode) const
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
    default:
    {
        DEBUG("Invalid opcode: {:X}", opcode);
        return Opcode6502::INVALID;
    }
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
            uint8_t pcOffset = memory_->Read(pc_);
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
            uint8_t pcOffset = memory_->Read(pc_);
            pc_++;
            pc_ = static_cast<uint16_t>(pc_ + pcOffset);
        }
        else {
            pc_++;
        }
        break;
    }
    case Opcode6502::INVALID:
    {
        DEBUG("Invalid opcode");
        assert(false);
        break;
    }
    default: break;
    }
}

void CPU6502::InputOpcode(const Opcode6502& opcode, uint8_t val)
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
    case Opcode6502::INVALID:
    {
        DEBUG("Invalid opcode");
        assert(false);
        break;
    }
    default:
    {
        break;
    }
    }
}

void CPU6502::InputOpcode(const Opcode6502& opcode, uint16_t val)
{
    switch (opcode) {
    case Opcode6502::STA:
    {
        memory_->Write(val, accumReg_);
        break;
    }
    case Opcode6502::LDA:
    {
        StoreDataAccumReg(memory_->Read(val));
        break;
    }
    case Opcode6502::INVALID:
    {
        DEBUG("Invalid opcode");
        assert(false);
        break;
    }
    default:
    {
        break;
    }
    }
}

void CPU6502::IncreasePC(uint16_t offset)
{
    pc_ += offset;
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
        "{:<9}{:<9}{:<9}{:<9}{:<9}\n", "PC", "SP", "Accum", "X", "Y");
    fmtStr += fmt::format(
        "{:<9X}{:<9X}{:<9X}{:<9X}{:<9X}\n", pc_, sp_, accumReg_, xReg_, yReg_);
    fmtStr += pStatus_->ToString();
    fmtStr += fmt::format("\n");
    fmtStr += memory_->ToString();
    return fmtStr;
}
