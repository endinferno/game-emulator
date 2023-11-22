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
    chip8_emulator emulator;
    chip8_window window;
    bool quit;
};
