#include <cassert>

#include "CPU6502.hpp"
#include "Logger.hpp"
#include "NesOpcode.hpp"

CPU6502::CPU6502(std::shared_ptr<NesReader>& nesReader)
    : pStatus_(std::make_shared<PStatusReg6502>())
    , memory_(std::make_shared<Memory6502>(nesReader))
    , accumReg_(pStatus_, 0)
    , xReg_(pStatus_, 0)
    , yReg_(pStatus_, 0)
{
    Reset();
}

void CPU6502::Reset()
{
    uint16_t startAddr = memory_->ReadWord(RESET_VECTOR_ADDR);
    Reset(startAddr);
}

void CPU6502::Reset(uint16_t startAddr)
{
    DEBUG("Reset CPU at address: {:X}\n", startAddr);
    accumReg_ = 0;
    xReg_ = 0;
    yReg_ = 0;
    sp_ = 0xFF;
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
    uint8_t opcodeVal = memory_->ReadByte(pc_);
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
        uint8_t val = memory_->ReadByte(pc_);
        accumReg_ = val;
        IncreasePC(1);
        break;
    }
    case Opcode6502::LDAAbsolute:
    {
        uint16_t addr = memory_->ReadWord(pc_);
        uint8_t val = memory_->ReadByte(addr);
        accumReg_ = val;
        IncreasePC(2);
        break;
    }
    case Opcode6502::LDXImmediate:
    {
        uint8_t val = memory_->ReadByte(pc_);
        xReg_ = val;
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
            int8_t pcOffset = static_cast<int8_t>(memory_->ReadByte(pc_));
            IncreasePC(1);
            pc_ = static_cast<uint16_t>(pc_ + pcOffset);
        } else {
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
          (uint8_t)accumReg_,
          addr);
    memory_->Write(addr, accumReg_);
}

std::string CPU6502::ToString() const
{
    std::string fmtStr;
    fmtStr += fmt::format("CPU Registers\n");
    fmtStr += fmt::format(
        "{:<9}{:<9}{:<9}{:<9}{:<9}\n", "PC", "SP", "Accum", "X", "Y");
    fmtStr += fmt::format("{:<9X}{:<9X}{:<9X}{:<9X}{:<9X}\n",
                          pc_,
                          sp_,
                          (uint8_t)accumReg_,
                          (uint8_t)xReg_,
                          (uint8_t)yReg_);
    fmtStr += pStatus_->ToString();
    fmtStr += fmt::format("\n");
    fmtStr += memory_->ToString();
    return fmtStr;
}
