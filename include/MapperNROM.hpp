#pragma once

#include <memory>

#include <cstdint>

#include "NesReader.hpp"

using std::uint8_t;

class MapperNROM
{
public:
    explicit MapperNROM(std::shared_ptr<NesReader>& nesReader);

    uint8_t read(const uint16_t& addr) const;
    void write(const uint16_t& addr, const uint8_t val);

    ~MapperNROM() = default;

private:
    constexpr static uint16_t ROM_SIZE = 0x4000;
    constexpr static uint16_t ROM_ADDR_OFFSET = 0x8000;
    const std::string mapperName;
    std::shared_ptr<NesReader> nesReader_;
};
