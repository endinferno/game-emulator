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
    chip8_opcode Fetch();
    void Decode(const chip8_opcode& opcode);
    void HandleKeyEvent(uint32_t keytype, int keycode);

    const std::vector<std::vector<uint8_t>>& GetScreen() const;

    bool IsWait() const;

    ~Chip8Emulator() = default;

private:
    void PushStack(uint16_t val);
    uint16_t PopStack();
    uint8_t GetRandom();
    void DecodeOpcode0(const chip8_opcode& opcode);
    void DecodeOpcode1(const chip8_opcode& opcode);
    void DecodeOpcode2(const chip8_opcode& opcode);
    void DecodeOpcode3(const chip8_opcode& opcode);
    void DecodeOpcode4(const chip8_opcode& opcode);
    void DecodeOpcode5(const chip8_opcode& opcode);
    void DecodeOpcode6(const chip8_opcode& opcode);
    void DecodeOpcode7(const chip8_opcode& opcode);
    void DecodeOpcode8(const chip8_opcode& opcode);
    void DecodeOpcode9(const chip8_opcode& opcode);
    void DecodeOpcodeA(const chip8_opcode& opcode);
    void DecodeOpcodeB(const chip8_opcode& opcode);
    void DecodeOpcodeC(const chip8_opcode& opcode);
    void DecodeOpcodeD(const chip8_opcode& opcode);
    void DecodeOpcodeE(const chip8_opcode& opcode);
    void DecodeOpcodeF(const chip8_opcode& opcode);

private:
    static const uint8_t CHIP8_STACK_SIZE = 0xFF;
    static const uint8_t CHIP8_REG_COUNT = 16;
    static const uint8_t CHIP8_KEY_COUNT = 16;
    uint8_t screen_width;
    uint8_t screen_height;

    uint16_t pc;
    uint8_t delay_timer;
    uint8_t sound_timer;
    uint16_t I_reg;
    std::vector<uint8_t> V_reg;
    std::vector<bool> key_state;

    std::vector<uint8_t> rom_content;
    std::stack<uint16_t> rom_stack;
    std::vector<std::vector<uint8_t>> screen;
    bool in_wait;
    std::vector<uint8_t>::iterator wait_reg;

    std::unordered_map<int, uint8_t> chip8_key_map;

    std::random_device seed;
    std::ranlux48 engine;
    std::uniform_int_distribution<> distrib;
};
