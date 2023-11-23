#include "Chip8Window.hpp"
#include "Logger.hpp"

Chip8Window::Chip8Window(const std::string& sdlName,
                         const std::pair<int32_t, int32_t>& sdlWindowSize)
    : sdlName_(sdlName)
    , sdlWidth_(sdlWindowSize.first)
    , sdlHeight_(sdlWindowSize.second)
{
    int32_t screenWidth = sdlWidth_ * SCALE;
    int32_t screenHeight = sdlHeight_ * SCALE;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        ERROR("Fail to Init SDL Video: {}", SDL_GetError());
        exit(1);
    }

    window_ = SDL_CreateWindow(sdlName.c_str(),
                               SDL_WINDOWPOS_CENTERED,
                               SDL_WINDOWPOS_CENTERED,
                               screenWidth,
                               screenHeight,
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
                                 screenWidth,
                                 screenHeight);
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
    uint32_t curTimerTick = SDL_GetTicks();
    if (curTimerTick - timerTick_ >= timerInterval_) {
        timerTick_ = curTimerTick;
        return true;
    }
    return false;
}

bool Chip8Window::UpdateRenderTick()
{
    uint32_t curRenderTick = SDL_GetTicks();
    if (curRenderTick - renderTick_ >= renderInterval_) {
        renderTick_ = curRenderTick;
        return true;
    }
    return false;
}

void Chip8Window::UpdateScreen(const Chip8Emulator& emulator)
{
    int pitch = 0;
    uint32_t* startPixels = nullptr;
    const std::vector<std::vector<uint8_t>>& screen = emulator.GetScreen();

    SDL_LockTexture(texture_, nullptr, (void**)&startPixels, &pitch);

    for (int32_t x = 0; x < sdlWidth_; x++) {
        for (int32_t y = 0; y < sdlHeight_; y++) {
            uint32_t color = (screen[x][y] == 0x0) ? 0x000000FF : 0xFFFFFFFF;
            ptrdiff_t ptrDiff = x * SCALE + y * SCALE * pitch / 4;
            uint32_t* pixels = startPixels + ptrDiff;
            if (pixels[0] == color) {
                continue;
            }
            FillBlock(pixels, pitch, color);
        }
    }

    SDL_UnlockTexture(texture_);
    SDL_RenderCopy(renderer_, texture_, nullptr, nullptr);
    SDL_RenderPresent(renderer_);
}

void Chip8Window::FillBlock(uint32_t* pixels, int pitch, uint32_t color)
{
    for (uint32_t y = 0; y < SCALE; y++) {
        for (uint32_t x = 0; x < SCALE; x++) {
            pixels[x + y * pitch / 4] = color;
        }
    }
}
