#pragma once

#include <cstdint>
#include <string>

#include <SDL2/SDL.h>

#include "Chip8Emulator.hpp"
#include "Chip8Window.hpp"

class Chip8App
{
public:
    Chip8App(const std::string& sdlName, uint8_t width, uint8_t height);

    bool LoadRom(const std::string& romPath);
    void Run();

    ~Chip8App() = default;

private:
    constexpr static int ROM_CONTENT_SIZE = 4096;
    Chip8Emulator emulator_;
    Chip8Window window_;
    bool isQuit_ = false;
};
