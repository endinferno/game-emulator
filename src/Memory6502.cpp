#include "Memory6502.hpp"
#include "NesReader.hpp"

Memory6502::Memory6502(std::shared_ptr<NesReader>& nesReader)
    : internalRam_(INTERNAL_RAM_SIZE, 0)
    , mapper_(nesReader)
{}

uint8_t Memory6502::read(const uint16_t& addr) const
{
    if (addr < INTERNAL_RAM_RANGE) {
        return internalRam_[addr & (INTERNAL_RAM_SIZE - 1)];
    }
    else if (addr < PPU_REG_RANGE) {
        return ppuReg_.read(addr);
    }
    return 0;
}

void Memory6502::write(const uint16_t& addr, const uint8_t val)
{
    if (addr < INTERNAL_RAM_RANGE) {
        internalRam_[addr & (INTERNAL_RAM_SIZE - 1)] = val;
    }
    else if (addr < PPU_REG_RANGE) {
        ppuReg_.write(addr, val);
    }
}

std::string Memory6502::ToString() const
{
    return ppuReg_.ToString();
}
