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
    DEBUG("Reset CPU at address: {:X}\n", startAddr);
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
    while (true) {
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
    case 0x78: return Opcode6502::SEI;
    case 0xD8: return Opcode6502::CLD;
    case 0x9A: return Opcode6502::TXS;
    case 0x10: return Opcode6502::BPL;
    case 0xA9: return Opcode6502::LDAImmediate;
    case 0xAD: return Opcode6502::LDAAbsolute;
    case 0xA2: return Opcode6502::LDXImmediate;
    case 0x8D: return Opcode6502::STAAbsolute;
    default:
    {
        DEBUG("Invalid opcode: {:X}\n", opcode);
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
    case Opcode6502::LDAImmediate:
    {
        LoadDataAccumReg(memory_->Read(pc_));
        IncreasePC(1);
        break;
    }
    case Opcode6502::LDAAbsolute:
    {
        uint16_t addr = memory_->ReadWord(pc_);
        LoadDataAccumReg(memory_->Read(addr));
        IncreasePC(2);
        break;
    }
    case Opcode6502::LDXImmediate:
    {
        LoadDataXReg(memory_->Read(pc_));
        IncreasePC(1);
        break;
    }
    case Opcode6502::STAAbsolute:
    {
        StoreAccumRegMemory(memory_->ReadWord(pc_));
        IncreasePC(2);
        break;
    }
    case Opcode6502::BPL:
    {
        if (!pStatus_->GetNegativeFlag()) {
            int8_t pcOffset = memory_->Read(pc_);
            IncreasePC(1);
            pc_ = static_cast<uint16_t>(pc_ + pcOffset);
        }
        else {
            IncreasePC(1);
        }
        break;
    }
    case Opcode6502::INVALID:
    {
        DEBUG("Invalid opcode\n");
        assert(false);
        break;
    }
    default:
    {
        DEBUG("Unknown opcode\n");
        assert(false);
        break;
    }
    }
}

void CPU6502::IncreasePC(uint16_t offset)
{
    pc_ += offset;
}

void CPU6502::StoreAccumRegMemory(uint16_t addr)
{
    DEBUG("Store Accumulator Register 0x{:0>2X} in memory: 0x{:0>4X}\n",
          accumReg_,
          addr);
    memory_->Write(addr, accumReg_);
}

void CPU6502::LoadDataXReg(uint8_t val)
{
    DEBUG("Load data to X Register: 0x{:0>2X}\n", val);
    pStatus_->SetZeroFlag(val == 0);
    pStatus_->SetNegativeFlag((val & 0x80) != 0);
    xReg_ = val;
}

void CPU6502::LoadDataAccumReg(uint8_t val)
{
    DEBUG("Load data to Accumulator Register: 0x{:0>2X}\n", val);
    pStatus_->SetZeroFlag(val == 0);
    pStatus_->SetNegativeFlag((val & 0x80) != 0);
    accumReg_ = val;
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
