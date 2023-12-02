#include "NesReader.hpp"
#include "Logger.hpp"

NesReader::NesReader()
    : nesHeader_(std::make_shared<NesHeader>())
    , nesRom_(std::make_shared<std::vector<uint8_t>>())
    , nesvRom_(std::make_shared<std::vector<uint8_t>>())
{}

NesReader::NesReader(const std::string& romFilePath)
    : NesReader()
{
    Open(romFilePath);
}

bool NesReader::Open(const std::string& romFilePath)
{
    DEBUG("Open Rom File: {}\n", romFilePath);
    romFileName_ = romFilePath;
    std::ifstream romFile;
    romFile.open(romFilePath, std::ios::in | std::ios::binary);
    if (!romFile.is_open()) {
        ERROR("Fail to open file {}\n", romFileName_);
        return false;
    }

    romFileSize_ = ReadLength(romFile);
    DEBUG("Rom File Length: {}\n", romFileSize_);
    if (romFileSize_ < NES_HEADER_SIZE) {
        ERROR("Invalid NES file format\n");
        return false;
    }
    auto buffer = ReadRom(romFile, 0, NES_HEADER_SIZE);
    nesHeader_->ReadNes(buffer);

    uint8_t numRomBanks = nesHeader_->GetNumRomBanks();
    DEBUG("Rom Banks Count: {}\n", numRomBanks);
    size_t romOffset = NES_HEADER_SIZE;
    size_t romBankSize = numRomBanks * NES_ROM_BANK_SIZE;
    if (romFileSize_ < NES_HEADER_SIZE + romBankSize) {
        ERROR("Invalid NES file format\n");
        return false;
    }
    nesRom_ = ReadRom(romFile, romOffset, romBankSize);

    uint8_t numVRomBanks = nesHeader_->GetNumVRomBanks();
    DEBUG("VRom Banks Count: {}\n", numVRomBanks);
    size_t vRomOffset = NES_HEADER_SIZE + romOffset;
    size_t vRomBankSize = numVRomBanks * NES_VROM_BANK_SIZE;
    if (romFileSize_ < NES_HEADER_SIZE + romBankSize + vRomBankSize) {
        ERROR("Invalid NES file format\n");
        return false;
    }
    nesvRom_ = ReadRom(romFile, vRomOffset, vRomBankSize);

    romFile.close();
    return true;
}

std::shared_ptr<NesHeader> NesReader::GetHeader() const
{
    return nesHeader_;
}

std::shared_ptr<std::vector<uint8_t>> NesReader::GetRom() const
{
    return nesRom_;
}

std::shared_ptr<std::vector<uint8_t>> NesReader::GetVRom() const
{
    return nesvRom_;
}

std::string NesReader::ToString() const
{
    return fmt::format("NES Rom\n"
                       "{:<13}: {}\n"
                       "{:<13}: {}\n"
                       "{:<13}: {}\n"
                       "{:<13}: {}\n"
                       "{}",
                       "File Name",
                       romFileName_,
                       "File Size",
                       romFileSize_,
                       "Rom Length",
                       nesRom_->size(),
                       "VRom Length",
                       nesvRom_->size(),
                       nesHeader_->ToString());
}

size_t NesReader::ReadLength(std::ifstream& romFile) const
{
    romFile.seekg(0, std::ios::end);
    return static_cast<size_t>(romFile.tellg());
}

std::shared_ptr<std::vector<uint8_t>> NesReader::ReadRom(std::ifstream& romFile,
                                                         size_t offset,
                                                         size_t size) const
{
    DEBUG("Read file offset: {} size: {}\n", offset, size);
    std::shared_ptr<std::vector<uint8_t>> readBuffer =
        std::make_shared<std::vector<uint8_t>>();
    readBuffer->resize(size);
    romFile.seekg(offset, std::ios::beg);
    romFile.read(reinterpret_cast<char*>(readBuffer->data()), size);
    return readBuffer;
}
