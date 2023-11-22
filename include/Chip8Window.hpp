#pragma once

#include <SDL2/SDL.h>

#include "Chip8Emulator.hpp"

class Chip8Window
{
public:
    Chip8Window(const std::string& sdl_name, uint32_t sdl_width,
                 uint32_t sdl_height);

    bool UpdateTimerTick();
    bool UpdateRenderTick();
    void UpdateScreen(const Chip8Emulator& emulator);
    void FillBlock(uint32_t* pixels, int pitch, uint32_t color);

    ~Chip8Window() = default;

private:
    static const uint32_t scale = 10;
    uint32_t sdl_width;
    uint32_t sdl_height;
    std::string SDL_name;
    uint32_t render_tick, render_interval;
    uint32_t timer_tick, timer_interval;

    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
};
