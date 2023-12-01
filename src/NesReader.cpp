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
    DEBUG("Open Rom File: {}", romFilePath);
    romFileName_ = romFilePath;
    std::ifstream romFile;
    romFile.open(romFilePath, std::ios::in | std::ios::binary);
    if (!romFile.is_open()) {
        ERROR("Fail to open file {}", romFileName_);
        return false;
    }

    size_t romLength = ReadLength(romFile);
    DEBUG("Rom File Length: {}", romLength);
    if (romLength < NES_HEADER_SIZE) {
        ERROR("Invalid NES file format");
        return false;
    }
    auto buffer = ReadRom(romFile, 0, NES_HEADER_SIZE);
    nesHeader_->ReadNes(buffer);

    uint8_t numRomBanks = nesHeader_->GetNumRamBanks();
    DEBUG("Rom Banks Count: {}", numRomBanks);
    size_t romOffset = NES_HEADER_SIZE;
    size_t romBankSize = numRomBanks * NES_ROM_BANK_SIZE;
    if (romLength < NES_HEADER_SIZE + romBankSize) {
        ERROR("Invalid NES file format");
        return false;
    }
    nesRom_ = ReadRom(romFile, romOffset, romBankSize);

    uint8_t numVRomBanks = nesHeader_->GetNumVRomBanks();
    DEBUG("VRom Banks Count: {}", numVRomBanks);
    size_t vRomOffset = NES_HEADER_SIZE + romOffset;
    size_t vRomBankSize = numVRomBanks * NES_VROM_BANK_SIZE;
    if (romLength < NES_HEADER_SIZE + romBankSize + vRomBankSize) {
        ERROR("Invalid NES file format");
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
    std::string fmtStr = "";
    fmtStr += "Rom File Name: " + romFileName_ + "\n";
    fmtStr += "Rom File Length: " + fmt::format("{}", nesRom_->size()) + "\n";
    fmtStr += "VRom File Length: " + fmt::format("{}", nesvRom_->size()) + "\n";
    fmtStr += nesHeader_->ToString();
    return fmtStr;
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
    DEBUG("Read file offset: {} size: {}", offset, size);
    std::shared_ptr<std::vector<uint8_t>> readBuffer =
        std::make_shared<std::vector<uint8_t>>();
    readBuffer->resize(size);
    romFile.seekg(offset, std::ios::beg);
    romFile.read(reinterpret_cast<char*>(readBuffer->data()), size);
    return readBuffer;
}
