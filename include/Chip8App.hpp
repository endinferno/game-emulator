#pragma once

#include <cstdint>
#include <string>

#include <SDL2/SDL.h>

#include "Chip8Emulator.hpp"
#include "Chip8Window.hpp"

class Chip8App
{
public:
    Chip8App(const std::string& sdl_name, uint8_t width, uint8_t height);

    bool LoadRom(const std::string& rom_path);
    void Run();

    ~Chip8App() = default;

private:
    Chip8Emulator emulator;
    Chip8Window window;
    bool quit;
};
