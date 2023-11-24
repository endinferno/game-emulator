#include "Chip8Window.hpp"

Chip8Window::Chip8Window(const std::string& winName,
                         const std::pair<uint32_t, uint32_t>& winSize)
    : winName_(winName)
    , winWidth_(winSize.first)
    , winHeight_(winSize.second)
    , pixelLocTable_(winWidth_, std::vector<std::pair<int32_t, int32_t>>(
                                    winHeight_, { 0, 0 }))
    , window_(sf::VideoMode({ winWidth_ * SCALE, winHeight_ * SCALE }),
              winName_)
    , renderedPixel_({ SCALE, SCALE })
{
    for (uint32_t i = 0; i < winWidth_; i++) {
        for (uint32_t j = 0; j < winHeight_; j++) {
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
    auto curRenderTick = clock_.getElapsedTime().asMilliseconds();
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
    for (uint32_t x = 0; x < winWidth_; x++) {
        for (uint32_t y = 0; y < winHeight_; y++) {
            if (screen[x][y] == 0x0) {
                continue;
            }
            auto p = pixelLocTable_[x][y];
            renderedPixel_.setOrigin(
                { static_cast<float>(p.first), static_cast<float>(p.second) });
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
