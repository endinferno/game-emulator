#include <fstream>

#include "Chip8App.hpp"
#include "Logger.hpp"

Chip8App::Chip8App(const std::string& sdlName, uint8_t width, uint8_t height)
    : emulator_(width, height)
    , window_(sdlName, { width, height })
{}

bool Chip8App::LoadRom(const std::string& romPath)
{
    std::array<uint8_t, ROM_CONTENT_SIZE> romContent;
    uint16_t romContentLen = 0;
    std::ifstream rom(romPath, std::ios::in | std::ios::binary);
    if (!rom) {
        ERROR("Fail to read rom file");
        return false;
    }
    rom.seekg(0, std::ios::end);
    romContentLen = rom.tellg();
    rom.seekg(0, std::ios::beg);

    rom.read((char*)romContent.data() + 0x200, romContentLen);
    rom.close();

    emulator_.SetRom(romContent.data(), romContentLen + 0x200);

    return true;
}

void Chip8App::Run()
{
    SDL_Event e;

    while (!isQuit_) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                isQuit_ = true;
            }
            emulator_.HandleKeyEvent(e);
        }
        if (window_.UpdateTimerTick()) {
            emulator_.DecreaseDelayTimer();
        }
        if (!emulator_.IsWait()) {
            Chip8Opcode opcode = emulator_.Fetch();
            emulator_.Decode(opcode);
        }
        SDL_Delay(1);
        if (window_.UpdateRenderTick()) {
            window_.UpdateScreen(emulator_);
        }
    }
}
