#pragma once

#include <SDL2/SDL.h>

#include "Chip8Emulator.hpp"

class Chip8Window
{
public:
    Chip8Window(const std::string& sdlName,
                const std::pair<int32_t, int32_t>& sdlWindowSize);

    bool UpdateTimerTick();
    bool UpdateRenderTick();
    void UpdateScreen(const Chip8Emulator& emulator);
    void FillBlock(uint32_t* pixels, int pitch, uint32_t color);

    ~Chip8Window() = default;

private:
    constexpr static int32_t SCALE = 10;
    std::string sdlName_;
    int32_t sdlWidth_;
    int32_t sdlHeight_;
    uint32_t renderTick_, renderInterval_;
    uint32_t timerTick_, timerInterval_;

    SDL_Window* window_;
    SDL_Renderer* renderer_;
    SDL_Texture* texture_;
};
