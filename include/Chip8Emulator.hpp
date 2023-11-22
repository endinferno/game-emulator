#pragma once

#include <random>
#include <stack>
#include <unordered_map>
#include <vector>

#include <SDL2/SDL.h>

#include "Chip8Opcode.hpp"

class Chip8Emulator
{
public:
    Chip8Emulator(uint8_t width, uint8_t height);

    void SetRom(uint8_t* content, uint16_t len);
    void DecreaseDelayTimer();
    Chip8Opcode Fetch();
    void Decode(const Chip8Opcode& opcode);
    void HandleKeyEvent(uint32_t keytype, int keycode);

    const std::vector<std::vector<uint8_t>>& GetScreen() const;

    bool IsWait() const;

    ~Chip8Emulator() = default;

private:
    void PushStack(uint16_t val);
    uint16_t PopStack();
    uint8_t GetRandom();
    void DecodeOpcode0(const Chip8Opcode& opcode);
    void DecodeOpcode1(const Chip8Opcode& opcode);
    void DecodeOpcode2(const Chip8Opcode& opcode);
    void DecodeOpcode3(const Chip8Opcode& opcode);
    void DecodeOpcode4(const Chip8Opcode& opcode);
    void DecodeOpcode5(const Chip8Opcode& opcode);
    void DecodeOpcode6(const Chip8Opcode& opcode);
    void DecodeOpcode7(const Chip8Opcode& opcode);
    void DecodeOpcode8(const Chip8Opcode& opcode);
    void DecodeOpcode9(const Chip8Opcode& opcode);
    void DecodeOpcodeA(const Chip8Opcode& opcode);
    void DecodeOpcodeB(const Chip8Opcode& opcode);
    void DecodeOpcodeC(const Chip8Opcode& opcode);
    void DecodeOpcodeD(const Chip8Opcode& opcode);
    void DecodeOpcodeE(const Chip8Opcode& opcode);
    void DecodeOpcodeF(const Chip8Opcode& opcode);

private:
    constexpr static uint8_t CHIP8_STACK_SIZE = 0xFF;
    constexpr static uint8_t CHIP8_REG_COUNT = 16;
    constexpr static uint8_t CHIP8_KEY_COUNT = 16;
    uint8_t screenWidth_;
    uint8_t screenHeight_;

    uint16_t pc_;
    uint8_t delayTimer_;
    uint8_t soundTimer_;
    uint16_t iReg_;
    std::vector<uint8_t> vReg_;
    std::vector<bool> keyState_;

    std::vector<uint8_t> romContent_;
    std::stack<uint16_t> romStack_;
    std::vector<std::vector<uint8_t>> screen_;
    bool inWait_;
    std::vector<uint8_t>::iterator waitReg_;

    std::unordered_map<int, uint8_t> chip8KeyMap_;

    std::random_device seed_;
    std::ranlux48 engine_;
    std::uniform_int_distribution<> distrib_;
};
