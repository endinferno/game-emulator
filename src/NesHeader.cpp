#include "NesHeader.hpp"
#include "Logger.hpp"
#include "fmt/core.h"

bool NesHeader::ReadNes(std::shared_ptr<std::vector<uint8_t>>& nesBuffer)
{
    DEBUG("NES Buffer Size: {}", nesBuffer->size());
    numRomBanks_ = nesBuffer->at(4);
    numVRomBanks_ = nesBuffer->at(5);
    mirroringType_ = nesBuffer->at(6) & 0x1;
    batteryBackedRam_ = (nesBuffer->at(6) & 0x2) >> 1;
    trainer_ = (nesBuffer->at(6) & 0x4) >> 2;
    fourScreenvRam_ = (nesBuffer->at(6) & 0x8) >> 3;
    VSUnisystem_ = nesBuffer->at(7) & 0x1;
    numRamBanks_ = nesBuffer->at(8);
    TVSystem_ = nesBuffer->at(9) & 0x1;
    mapperType_ = (nesBuffer->at(7) & 0xF0) | (nesBuffer->at(6) >> 4);

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

uint8_t NesHeader::GetFourScreenvRam() const
{
    return fourScreenvRam_;
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

std::string NesHeader::ToString() const
{
    std::string fmtStr;
    fmtStr += fmt::format("{:<15} : {}\n", "Num Rom Banks", numRomBanks_);
    fmtStr += fmt::format("{:<15} : {}\n", "Num VRom Banks", numVRomBanks_);
    fmtStr += fmt::format("{:<15} : {}\n", "Mirroring Type", mirroringType_);
    fmtStr +=
        fmt::format("{:<15} : {}\n", "Battery Backed Ram", batteryBackedRam_);
    fmtStr += fmt::format("{:<15} : {}\n", "Trainer", trainer_);
    fmtStr += fmt::format("{:<15} : {}\n", "Four Screen vRam", fourScreenvRam_);
    fmtStr += fmt::format("{:<15} : {}\n", "VS Unisystem", VSUnisystem_);
    fmtStr += fmt::format("{:<15} : {}\n", "Num Ram Banks", numRamBanks_);
    fmtStr += fmt::format("{:<15} : {}\n", "TV System", TVSystem_);
    fmtStr += fmt::format("{:<15} : {}\n", "Mapper Type", mapperType_);


    return fmtStr;
}
