#pragma once

#include <fstream>
#include <vector>

#include <cstdint>

#include "NesHeader.hpp"

using std::uint8_t;

class NesReader
{
public:
    NesReader();
    NesReader(const std::string& romFilePath);

    bool Open(const std::string& romFilePath);
    std::shared_ptr<NesHeader> GetHeader() const;
    std::shared_ptr<std::vector<uint8_t>> GetRom() const;
    std::shared_ptr<std::vector<uint8_t>> GetVRom() const;
    std::string ToString() const;

    ~NesReader() = default;

private:
    size_t ReadLength(std::ifstream& romFile) const;
    std::shared_ptr<std::vector<uint8_t>> ReadRom(std::ifstream& romFile,
                                                  size_t offset,
                                                  size_t size) const;

private:
    constexpr static size_t NES_HEADER_SIZE = 16;
    constexpr static size_t NES_ROM_BANK_SIZE = 16 * 1024;
    constexpr static size_t NES_VROM_BANK_SIZE = 8 * 1024;

    std::string romFileName_;
    size_t romFileSize_;
    std::shared_ptr<NesHeader> nesHeader_;
    std::shared_ptr<std::vector<uint8_t>> nesRom_;
    std::shared_ptr<std::vector<uint8_t>> nesvRom_;
};
