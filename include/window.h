#ifndef __SDL_WINDOW_H
#define __SDL_WINDOW_H

#include <iostream>
#include <SDL2/SDL.h>
#include "emulator.h"

class chip8_window
{
public:
    chip8_window(const std::string& sdl_name, uint32_t sdl_width,
                 uint32_t sdl_height);

    bool update_timer_tick();
    bool update_render_tick();
    void update_screen(const chip8_emulator& emulator);
    void fill_block(uint32_t x, uint32_t y, void* raw_pixels, int pitch,
                    uint32_t color);

	~chip8_window();

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

#endif /* __SDL_WINDOW_H */
