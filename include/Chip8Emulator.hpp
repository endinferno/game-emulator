#pragma once

#include <random>
#include <stack>
#include <unordered_map>
#include <vector>

#include <SFML/Window.hpp>

#include "Chip8Opcode.hpp"

class Chip8Emulator
{
public:
    Chip8Emulator(uint8_t width, uint8_t height);

    void SetRom(uint8_t* content, uint16_t len);
    void DecreaseDelayTimer();
    Chip8Opcode Fetch();
    void Decode(const Chip8Opcode& opcode);
    void HandleEvent(const sf::Event& event);

    const std::vector<std::vector<uint8_t>>& GetScreen() const;

    bool IsWait() const;
    bool IsQuit() const;

    ~Chip8Emulator() = default;

private:
    void HandleCloseEvent(const sf::Event& event);
    void HandleKeyEvent(const sf::Event& event);
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
    std::array<uint8_t, CHIP8_REG_COUNT> vReg_;
    std::array<bool, CHIP8_KEY_COUNT> keyState_;

    std::vector<uint8_t> romContent_;
    std::stack<uint16_t> romStack_;
    std::vector<std::vector<uint8_t>> screen_;
    bool inWait_ = false;
    bool isQuit_ = false;
    std::array<uint8_t, CHIP8_REG_COUNT>::iterator waitReg_;

    std::random_device seed_;
    std::ranlux48 engine_;
    std::uniform_int_distribution<> distrib_;

    std::unordered_map<sf::Keyboard::Key, uint8_t> chip8KeyMap_ = {
        { sf::Keyboard::Num1, 0x1 }, { sf::Keyboard::Num2, 0x8 },
        { sf::Keyboard::Num3, 0x3 }, { sf::Keyboard::Num4, 0xC },
        { sf::Keyboard::Q, 0x4 },    { sf::Keyboard::W, 0x5 },
        { sf::Keyboard::E, 0x6 },    { sf::Keyboard::R, 0xD },
        { sf::Keyboard::A, 0x7 },    { sf::Keyboard::S, 0x2 },
        { sf::Keyboard::D, 0x9 },    { sf::Keyboard::F, 0xE },
        { sf::Keyboard::Z, 0xA },    { sf::Keyboard::X, 0x0 },
        { sf::Keyboard::C, 0xB },    { sf::Keyboard::V, 0xF },
    };
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
