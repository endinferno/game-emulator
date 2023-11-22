#include <fstream>

#include "Chip8App.hpp"
#include "Logger.hpp"

Chip8App::Chip8App(const std::string& sdl_name, uint8_t width, uint8_t height)
    : emulator_(width, height)
    , window_(sdl_name, width, height)
    , isQuit_(false)
{}

bool Chip8App::LoadRom(const std::string& rom_path)
{
    uint8_t rom_content[4096] = { 0 };
    uint16_t rom_content_len = 0;
    std::ifstream rom(rom_path, std::ios::in | std::ios::binary);
    if (!rom) {
        ERROR("Fail to read rom file");
        return false;
    }
    rom.seekg(0, std::ios::end);
    rom_content_len = rom.tellg();
    rom.seekg(0, std::ios::beg);

    rom.read((char*)rom_content + 0x200, rom_content_len);
    rom.close();

    emulator_.SetRom(rom_content, rom_content_len + 0x200);

    return true;
}

void Chip8App::Run()
{
    SDL_Event e;

    while (!isQuit_) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) isQuit_ = true;
            emulator_.HandleKeyEvent(e.type, e.key.keysym.sym);
        }
        if (window_.UpdateTimerTick()) emulator_.DecreaseDelayTimer();
        if (!emulator_.IsWait()) {
            Chip8Opcode opcode = emulator_.Fetch();
            emulator_.Decode(opcode);
        }
        SDL_Delay(1);
        if (window_.UpdateRenderTick()) window_.UpdateScreen(emulator_);
    }
}
