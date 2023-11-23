#pragma once

#include <optional>

#include <SFML/Graphics.hpp>

#include "Chip8Emulator.hpp"

class Chip8Window
{
public:
    Chip8Window(const std::string& winName,
                const std::pair<int32_t, int32_t>& winSize);

    bool UpdateTimerTick();
    bool UpdateRenderTick();
    void UpdateScreen(const Chip8Emulator& emulator);
    std::optional<sf::Event> PollEvent();

    ~Chip8Window();

private:
    constexpr static int32_t SCALE = 10;
    std::string winName_;
    int32_t winWidth_;
    int32_t winHeight_;
    int32_t renderTick_, renderInterval_;
    int32_t timerTick_, timerInterval_;

    std::vector<std::vector<std::pair<int32_t, int32_t>>> pixelLocTable_;
    sf::RenderWindow window_;
    sf::Clock clock_;
    sf::RectangleShape renderedPixel_;
};
