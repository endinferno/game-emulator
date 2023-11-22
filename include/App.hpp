#pragma once

#include <cstdint>
#include <fstream>
#include <string>

#include <SDL2/SDL.h>

#include "Emulator.hpp"
#include "Logger.hpp"
#include "Window.hpp"

class chip8_app
{
public:
    chip8_app(const std::string& sdl_name, uint8_t width, uint8_t height);

    bool load_rom(const std::string& rom_path);
    void run();

    ~chip8_app() = default;

private:
    chip8_emulator emulator;
    chip8_window window;
    bool quit;
};
