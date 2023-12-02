#include "Memory6502.hpp"
#include "Logger.hpp"
#include "NesReader.hpp"

Memory6502::Memory6502(std::shared_ptr<NesReader>& nesReader)
    : internalRam_(INTERNAL_RAM_SIZE, 0)
    , mapper_(nesReader)
{}

void Memory6502::Reset()
{
    std::fill(internalRam_.begin(), internalRam_.end(), 0);
    ppuReg_.Reset();
}

uint8_t Memory6502::ReadByte(const uint16_t& addr) const
{
    uint8_t readByte = Read(addr);
    DEBUG("ReadByte addr: 0x{:0>4X} value: 0x{:0>2X}\n", addr, readByte);
    return readByte;
}

uint16_t Memory6502::ReadWord(const uint16_t& addr) const
{
    uint16_t lowerByte = Read(addr);
    uint16_t upperByte = Read(addr + 1);
    DEBUG("ReadWord addr: 0x{:0>4X} value: 0x{:0>4X}\n",
          addr,
          lowerByte | (upperByte << 8));
    return lowerByte | (upperByte << 8);
}

void Memory6502::Write(const uint16_t& addr, const uint8_t val)
{
    DEBUG("Write addr: 0x{:0>4X} val: 0x{:0>2X}\n", addr, val);
    if (addr < INTERNAL_RAM_RANGE) {
        DEBUG("Write to internal RAM\n");
        internalRam_[addr & (INTERNAL_RAM_SIZE - 1)] = val;
    }
    else if (addr < PPU_REG_RANGE) {
        DEBUG("Write to PPU Register\n");
        ppuReg_.Write(addr, val);
    }
}

uint8_t Memory6502::Read(const uint16_t& addr) const
{
    DEBUG("Read addr: 0x{:0>4X}\n", addr);
    if (addr < INTERNAL_RAM_RANGE) {
        DEBUG("Read from internal RAM\n");
        return internalRam_[addr & (INTERNAL_RAM_SIZE - 1)];
    }
    else if (addr < PPU_REG_RANGE) {
        DEBUG("Read from PPU Register\n");
        return ppuReg_.Read(addr);
    }
    else if (addr < APU_REG_RANGE) {
        // TODO: Implement APU
    }
    else if (addr < PRG_RAM_RANGE) {
        // TODO: Implement PRG RAM
    }
    else {
        return mapper_.Read(addr);
    }
    return 0;
}

std::string Memory6502::ToString() const
{
    return ppuReg_.ToString();
}
