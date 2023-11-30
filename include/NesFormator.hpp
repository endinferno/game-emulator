#pragma once

#include <cstdint>
#include <vector>

#include "NesHeader.hpp"
#include "RomReader.hpp"

using std::uint8_t;

class NesFormator
{
public:
    explicit NesFormator(const std::string& romFilePath);

    NesHeader ReadHeader();
    std::vector<uint8_t> ReadRom() const;
    std::vector<uint8_t> ReadVRom() const;

    ~NesFormator() = default;

private:
    constexpr static size_t NES_ROM_OFFSET = 16;
    constexpr static size_t NES_ROM_BANK_SIZE = 16 * 1024;
    std::shared_ptr<RomReader> romReader_;
    NesHeader nesHeader_;
};
