#ifndef __CHIP8_APP_H__
#define __CHIP8_APP_H__

#include <string>
#include <cstdint>
#include <SDL2/SDL.h>

#include "Window.hpp"
#include "Emulator.hpp"
#include "Logger.hpp"

class chip8_app
{
public:
    chip8_app(const std::string& sdl_name, uint8_t width, uint8_t height);

    bool load_rom(const std::string& rom_path);
    void run();

    ~chip8_app();

private:
    chip8_emulator emulator;
    chip8_window window;
    bool quit;
};

#endif /* __CHIP8_APP_H__ */
