#include "Chip8Window.hpp"
#include "Logger.hpp"

Chip8Window::Chip8Window(const std::string& sdl_name, uint32_t sdl_width,
                           uint32_t sdl_height)
    : sdlName_(sdl_name)
    , sdlWidth_(sdl_width)
    , sdlHeight_(sdl_height)
{
    uint32_t screen_width = sdl_width * SCALE;
    uint32_t screen_height = sdl_height * SCALE;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        ERROR("Fail to Init SDL Video: {}", SDL_GetError());
        exit(1);
    }

    window_ = SDL_CreateWindow(sdl_name.c_str(),
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              screen_width,
                              screen_height,
                              SDL_WINDOW_SHOWN);
    if (window_ == nullptr) {
        ERROR("Fail to Create SDL Window: {}", SDL_GetError());
        exit(1);
    }

    uint32_t flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    renderer_ = SDL_CreateRenderer(window_, -1, flags);
    if (renderer_ == nullptr) {
        ERROR("Fail to Create Renderer: {}", SDL_GetError());
        exit(1);
    }

    texture_ = SDL_CreateTexture(renderer_,
                                SDL_PIXELFORMAT_RGBA8888,
                                SDL_TEXTUREACCESS_STREAMING,
                                screen_width,
                                screen_height);

    if (texture_ == nullptr) {
        ERROR("Fail to Create Texture: %s\n", SDL_GetError());
        exit(1);
    }
    renderTick_ = 0;
    renderInterval_ = 1000 / 60;

    timerTick_ = SDL_GetTicks();
    timerInterval_ = 1000 / 60;
}

bool Chip8Window::UpdateTimerTick()
{
    uint32_t cur_timer_tick = SDL_GetTicks();
    if (cur_timer_tick - timerTick_ >= timerInterval_) {
        timerTick_ = cur_timer_tick;
        return true;
    }
    return false;
}

bool Chip8Window::UpdateRenderTick()
{
    uint32_t cur_render_tick = SDL_GetTicks();
    if (cur_render_tick - renderTick_ >= renderInterval_) {
        renderTick_ = cur_render_tick;
        return true;
    }
    return false;
}

void Chip8Window::UpdateScreen(const Chip8Emulator& emulator)
{
    int pitch = 0;
    uint32_t* start_pixels = nullptr;
    const std::vector<std::vector<uint8_t>>& screen = emulator.GetScreen();

    SDL_LockTexture(texture_, nullptr, (void**)&start_pixels, &pitch);

    for (uint32_t x = 0; x < sdlWidth_; x++) {
        for (uint32_t y = 0; y < sdlHeight_; y++) {
            uint32_t color = (screen[x][y] == 0x0) ? 0x000000FF : 0xFFFFFFFF;
            uint32_t* pixels = start_pixels + x * SCALE + y * SCALE * pitch / 4;
            if (pixels[0] == color) continue;
            FillBlock(pixels, pitch, color);
        }
    }

    SDL_UnlockTexture(texture_);
    SDL_RenderCopy(renderer_, texture_, nullptr, nullptr);
    SDL_RenderPresent(renderer_);
}
void Chip8Window::FillBlock(uint32_t* pixels, int pitch, uint32_t color)
{
    for (uint32_t y = 0; y < SCALE; y++)
        for (uint32_t x = 0; x < SCALE; x++) pixels[x + y * pitch / 4] = color;
}
