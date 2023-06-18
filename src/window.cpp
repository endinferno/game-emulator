#include "window.h"

chip8_window::chip8_window(const std::string& sdl_name, uint32_t sdl_width,
                           uint32_t sdl_height)
    : SDL_name(sdl_name), sdl_width(sdl_width), sdl_height(sdl_height)
{
    uint32_t screen_width = sdl_width * scale;
    uint32_t screen_height = sdl_height * scale;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Fail to Init SDL Video: %s\n", SDL_GetError());
        exit(1);
    }

    window = SDL_CreateWindow(sdl_name.c_str(), SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED, screen_width,
                              screen_height, SDL_WINDOW_SHOWN);
    if (window == nullptr)
    {
        printf("Fail to Create SDL Window: %s\n", SDL_GetError());
        exit(1);
    }

    uint32_t flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    renderer = SDL_CreateRenderer(window, -1, flags);
    if (renderer == nullptr)
    {
        printf("Fail to Create Renderer: %s\n", SDL_GetError());
        exit(1);
    }

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                                SDL_TEXTUREACCESS_STREAMING, screen_width,
                                screen_height);

    if (texture == nullptr)
    {
        printf("Fail to Create Texture: %s\n", SDL_GetError());
        exit(1);
    }
    render_tick = 0;
    render_interval = 1000 / 60;

    timer_tick = SDL_GetTicks();
    timer_interval = 1000 / 60;
}

bool chip8_window::update_timer_tick()
{
    uint32_t cur_timer_tick = SDL_GetTicks();
    if (cur_timer_tick - timer_tick >= timer_interval)
    {
        timer_tick = cur_timer_tick;
        return true;
    }
    return false;
}

bool chip8_window::update_render_tick()
{
    uint32_t cur_render_tick = SDL_GetTicks();
    if (cur_render_tick - render_tick >= render_interval)
    {
        render_tick = cur_render_tick;
        return true;
    }
    return false;
}

void chip8_window::update_screen(const chip8_emulator& emulator)
{
    void* raw_pixels;
    int pitch;
    const std::vector<std::vector<uint8_t>>& screen = emulator.get_screen();

    SDL_LockTexture(texture, nullptr, &raw_pixels, &pitch);

    for (uint32_t x = 0; x < sdl_width; x++)
    {
        for (uint32_t y = 0; y < sdl_height; y++)
        {
            uint8_t pixel = screen[x][y];
            if (pixel == 0x0)
                fill_block(x * scale, y * scale, raw_pixels, pitch, 0x000000FF);
            else
                fill_block(x * scale, y * scale, raw_pixels, pitch, 0xFFFFFFFF);
        }
    }

    SDL_UnlockTexture(texture);
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}
void chip8_window::fill_block(uint32_t x, uint32_t y, void* raw_pixels,
                              int pitch, uint32_t color)
{
    uint32_t* pixels = (uint32_t*)raw_pixels + x + y * pitch / 4;

    for (uint32_t fill_y = 0; fill_y < scale; fill_y++)
        for (uint32_t fill_x = 0; fill_x < scale; fill_x++)
            pixels[fill_x + fill_y * pitch / 4] = color;
}

chip8_window::~chip8_window() {}
