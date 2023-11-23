#include "Chip8Window.hpp"
#include "Logger.hpp"

Chip8Window::Chip8Window(const std::string& sdlName,
                         const std::pair<int32_t, int32_t>& sdlWindowSize)
    : sdlName_(sdlName)
    , sdlWidth_(sdlWindowSize.first)
    , sdlHeight_(sdlWindowSize.second)
    , pixelLocTable_(sdlWidth_, std::vector<std::pair<int32_t, int32_t>>(
                                    sdlHeight_, { 0, 0 }))
    , window_(sf::VideoMode(sdlWidth_ * SCALE, sdlHeight_ * SCALE), sdlName_)
    , renderedPixel_({ SCALE, SCALE })
{
    for (int32_t i = 0; i < sdlWidth_; i++) {
        for (int32_t j = 0; j < sdlHeight_; j++) {
            pixelLocTable_[i][j] = { -i * SCALE, -j * SCALE };
        }
    }
    renderedPixel_.setFillColor(sf::Color::White);

    renderTick_ = 0;
    renderInterval_ = 1000 / 60;

    timerTick_ = clock_.getElapsedTime().asMicroseconds();
    timerInterval_ = 1000 / 60;
}

Chip8Window::~Chip8Window()
{
    window_.close();
}

bool Chip8Window::UpdateTimerTick()
{
    auto curTimerTick = clock_.getElapsedTime().asMilliseconds();
    if (curTimerTick - timerTick_ >= timerInterval_) {
        timerTick_ = curTimerTick;
        return true;
    }
    return false;
}

bool Chip8Window::UpdateRenderTick()
{
    auto curRenderTick = clock_.getElapsedTime().asMicroseconds();
    if (curRenderTick - renderTick_ >= renderInterval_) {
        renderTick_ = curRenderTick;
        return true;
    }
    return false;
}

void Chip8Window::UpdateScreen(const Chip8Emulator& emulator)
{
    const std::vector<std::vector<uint8_t>>& screen = emulator.GetScreen();

    window_.clear();
    for (int32_t x = 0; x < sdlWidth_; x++) {
        for (int32_t y = 0; y < sdlHeight_; y++) {
            if (screen[x][y] == 0x0) {
                continue;
            }
            auto p = pixelLocTable_[x][y];
            renderedPixel_.setOrigin(p.first, p.second);
            window_.draw(renderedPixel_);
        }
    }
    window_.display();
}

std::optional<sf::Event> Chip8Window::PollEvent()
{
    sf::Event event;
    if (window_.pollEvent(event)) {
        return event;
    }
    return std::nullopt;
}
