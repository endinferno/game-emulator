#include "NesHeader.hpp"
#include "Logger.hpp"

bool NesHeader::ReadNes(std::shared_ptr<RomReader>& romReader)
{
    auto buffer = romReader->GetRomContent();
    if (buffer->size() < 16) {
        ERROR("Invalid rom header");
        return false;
    }
    numRomBanks_ = buffer->at(4);
    numVRomBanks_ = buffer->at(5);
    mirroringType_ = buffer->at(6) & 0x1;
    batteryBackedRam_ = (buffer->at(6) & 0x2) >> 1;
    trainer_ = (buffer->at(6) & 0x4) >> 2;
    fourScreenVram_ = (buffer->at(6) & 0x8) >> 3;
    VSUnisystem_ = buffer->at(7) & 0x1;
    numRamBanks_ = buffer->at(8);
    TVSystem_ = buffer->at(9) & 0x1;
    mapperType_ = (buffer->at(7) & 0xF0) | (buffer->at(6) >> 4);

    return true;
}

uint8_t NesHeader::GetNumRomBanks() const
{
    return numRomBanks_;
}

uint8_t NesHeader::GetNumVRomBanks() const
{
    return numVRomBanks_;
}

uint8_t NesHeader::GetMirroringType() const
{
    return mirroringType_;
}

uint8_t NesHeader::GetBatteryBackedRam() const
{
    return batteryBackedRam_;
}

uint8_t NesHeader::GetTrainer() const
{
    return trainer_;
}

uint8_t NesHeader::GetFourScreenVram() const
{
    return fourScreenVram_;
}

uint8_t NesHeader::GetVSUnisystem() const
{
    return VSUnisystem_;
}

uint8_t NesHeader::GetNumRamBanks() const
{
    return numRamBanks_;
}

uint8_t NesHeader::GetTVSystem() const
{
    return TVSystem_;
}

uint8_t NesHeader::GetMapperType() const
{
    return mapperType_;
}
