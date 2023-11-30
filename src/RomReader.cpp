#include "RomReader.hpp"
#include "Logger.hpp"

bool RomReader::Open(const std::string& romFilePath)
{
    romFileName_ = romFilePath;
    romFile_.open(romFilePath, std::ios::in | std::ios::binary);
    if (!IsOpen()) {
        ERROR("Fail to open file {}", romFileName_);
        return false;
    }
    romFile_.seekg(0, std::ios::end);
    romLength_ = static_cast<size_t>(romFile_.tellg());

    romBuffer_->resize(romLength_);
    romFile_.seekg(0, std::ios::beg);
    romFile_.read(reinterpret_cast<char*>(romBuffer_->data()),
                  romBuffer_->size());
    return true;
}

RomReader::~RomReader()
{
    if (IsOpen()) {
        romFile_.close();
    }
}

size_t RomReader::GetLength() const
{
    if (!IsOpen()) {
        ERROR("File not open yet");
        return 0;
    }
    return romLength_;
}

std::shared_ptr<std::vector<uint8_t>> RomReader::GetRomContent() const
{
    if (!IsOpen()) {
        ERROR("File not open yet");
        return {};
    }
    return romBuffer_;
}

bool RomReader::IsOpen() const
{
    return romFile_.is_open();
}
