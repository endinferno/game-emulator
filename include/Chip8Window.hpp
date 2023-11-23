#pragma once

#include <optional>

#include <SFML/Graphics.hpp>

#include "Chip8Emulator.hpp"

class Chip8Window
{
public:
    Chip8Window(const std::string& sdlName,
                const std::pair<int32_t, int32_t>& sdlWindowSize);

    bool UpdateTimerTick();
    bool UpdateRenderTick();
    void UpdateScreen(const Chip8Emulator& emulator);
    std::optional<sf::Event> PollEvent();

    ~Chip8Window();

private:
    constexpr static int32_t SCALE = 10;
    std::string sdlName_;
    int32_t sdlWidth_;
    int32_t sdlHeight_;
    int32_t renderTick_, renderInterval_;
    int32_t timerTick_, timerInterval_;

    std::vector<std::vector<std::pair<int32_t, int32_t>>> pixelLocTable_;
    sf::RenderWindow window_;
    sf::Clock clock_;
    sf::RectangleShape renderedPixel_;
};
