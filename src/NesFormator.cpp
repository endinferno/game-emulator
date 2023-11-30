#include "NesFormator.hpp"
#include "Logger.hpp"

NesFormator::NesFormator(const std::string& romFilePath)
    : romReader_(std::make_shared<RomReader>())
{
    if (!romReader_->Open(romFilePath)) {
        ERROR("Fail to read rom file {}", romFilePath);
        return;
    }
    if (!nesHeader_.ReadNes(romReader_)) {
        ERROR("Fail to read Nes file header {}", romFilePath);
        return;
    }
}

NesHeader NesFormator::ReadHeader()
{
    return nesHeader_;
}

std::vector<uint8_t> NesFormator::ReadRom() const
{
    auto buffer = romReader_->GetRomContent();
    return std::vector<uint8_t>(buffer->begin() + NES_ROM_OFFSET,
                                buffer->end());
}

std::vector<uint8_t> NesFormator::ReadVRom() const
{
    auto buffer = romReader_->GetRomContent();
    auto numRomBanks = nesHeader_.GetNumRomBanks();
    size_t vromOffset = NES_ROM_BANK_SIZE +
                        static_cast<size_t>(numRomBanks) * NES_ROM_BANK_SIZE;
    return std::vector<uint8_t>(buffer->begin() + vromOffset, buffer->end());
}
