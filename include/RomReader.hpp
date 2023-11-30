#pragma once

#include <fstream>
#include <string>
#include <vector>

class RomReader
{
public:
    RomReader() = default;

    bool Open(const std::string& romFilePath);
    bool IsOpen() const;
    size_t GetLength() const;
    std::shared_ptr<std::vector<uint8_t>> GetRomContent() const;

    ~RomReader();

private:
    std::string romFileName_;
    std::ifstream romFile_;
    size_t romLength_;
    std::shared_ptr<std::vector<uint8_t>> romBuffer_;
};
