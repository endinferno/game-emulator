// Nes Format Reference: http://fms.komkon.org/EMUL8/NES.html#LABM
#pragma once

#include <memory>
#include <vector>

#include <cstdint>


using std::uint8_t;

class NesHeader
{
public:
    NesHeader() = default;

    bool ReadNes(std::shared_ptr<std::vector<uint8_t>>& nesBuffer);
    uint8_t GetNumRomBanks() const;
    uint8_t GetNumVRomBanks() const;
    uint8_t GetMirroringType() const;
    uint8_t GetBatteryBackedRam() const;
    uint8_t GetTrainer() const;
    uint8_t GetFourScreenvRam() const;
    uint8_t GetVSUnisystem() const;
    uint8_t GetNumRamBanks() const;
    uint8_t GetTVSystem() const;
    uint8_t GetMapperType() const;
    std::string ToString() const;

    ~NesHeader() = default;

private:
    uint8_t numRomBanks_;
    uint8_t numVRomBanks_;
    uint8_t mirroringType_;
    uint8_t batteryBackedRam_;
    uint8_t trainer_;
    uint8_t fourScreenvRam_;
    uint8_t VSUnisystem_;
    uint8_t numRamBanks_;
    uint8_t TVSystem_;
    uint8_t mapperType_;
};
