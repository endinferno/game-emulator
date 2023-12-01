#include "MapperNROM.hpp"
#include "Logger.hpp"

MapperNROM::MapperNROM(std::shared_ptr<NesReader>& nesReader)
    : mapperName("Mapper NROM")
    , nesReader_(nesReader)
{}

uint8_t MapperNROM::read(const uint16_t& addr) const
{
    uint16_t realAddr = 0;
    if (nesReader_->GetHeader()->GetNumRomBanks() == 1) {
        realAddr = (addr - ROM_ADDR_OFFSET) % (ROM_SIZE - 1);
    }
    else {
        realAddr = addr - ROM_ADDR_OFFSET;
    }
    uint8_t readByte = nesReader_->GetRom()->at(realAddr);
    DEBUG("{} read addr {:X} value {:X}", mapperName, addr, readByte);
    return readByte;
}

void MapperNROM::write(const uint16_t& addr, const uint8_t val)
{
    DEBUG("{} write addr {:X} value {:X}", mapperName, addr, val);
}
