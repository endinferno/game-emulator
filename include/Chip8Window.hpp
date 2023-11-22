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
    constexpr static uint32_t SCALE = 10;
    uint32_t sdlWidth_;
    uint32_t sdlHeight_;
    std::string sdlName_;
    uint32_t renderTick_, renderInterval_;
    uint32_t timerTick_, timerInterval_;

    SDL_Window* window_;
    SDL_Renderer* renderer_;
    SDL_Texture* texture_;
};
