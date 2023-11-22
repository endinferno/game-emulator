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
    void HandleKeyEvent(SDL_Event& event);

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

    uint16_t pc_ = 0x200;
    uint8_t delayTimer_;
    uint8_t soundTimer_;
    uint16_t iReg_;
    std::vector<uint8_t> vReg_;
    std::vector<bool> keyState_;

    std::vector<uint8_t> romContent_;
    std::stack<uint16_t> romStack_;
    std::vector<std::vector<uint8_t>> screen_;
    bool inWait_ = false;
    std::vector<uint8_t>::iterator waitReg_;

    std::unordered_map<int, uint8_t> chip8KeyMap_ = {
        { SDLK_1, 0x1 }, { SDLK_2, 0x8 }, { SDLK_3, 0x3 }, { SDLK_4, 0xC },
        { SDLK_q, 0x4 }, { SDLK_w, 0x5 }, { SDLK_e, 0x6 }, { SDLK_r, 0xD },
        { SDLK_a, 0x7 }, { SDLK_s, 0x2 }, { SDLK_d, 0x9 }, { SDLK_f, 0xE },
        { SDLK_z, 0xA }, { SDLK_x, 0x0 }, { SDLK_c, 0xB }, { SDLK_v, 0xF },
    };

    std::random_device seed_;
    std::ranlux48 engine_;
    std::uniform_int_distribution<> distrib_;
    std::array<uint8_t, 80> fontSet_ = {
        0xF0, 0x90, 0x90, 0x90, 0xF0, 0x20, 0x60, 0x20, 0x20, 0x70, 0xF0, 0x10,
        0xF0, 0x80, 0xF0, 0xF0, 0x10, 0xF0, 0x10, 0xF0, 0xA0, 0xA0, 0xF0, 0x20,
        0x20, 0xF0, 0x80, 0xF0, 0x10, 0xF0, 0xF0, 0x80, 0xF0, 0x90, 0xF0, 0xF0,
        0x10, 0x20, 0x40, 0x40, 0xF0, 0x90, 0xF0, 0x90, 0xF0, 0xF0, 0x90, 0xF0,
        0x10, 0xF0, 0xF0, 0x90, 0xF0, 0x90, 0x90, 0xE0, 0x90, 0xE0, 0x90, 0xE0,
        0xF0, 0x80, 0x80, 0x80, 0xF0, 0xE0, 0x90, 0x90, 0x90, 0xE0, 0xF0, 0x80,
        0xF0, 0x80, 0xF0, 0xF0, 0x80, 0xF0, 0x80, 0x80,
    };
};
