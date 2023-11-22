#include <fstream>

#include "Chip8App.hpp"
#include "Logger.hpp"

Chip8App::Chip8App(const std::string& sdl_name, uint8_t width, uint8_t height)
    : emulator(width, height)
    , window(sdl_name, width, height)
    , quit(false)
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

    emulator.SetRom(rom_content, rom_content_len + 0x200);

    return true;
}

void Chip8App::Run()
{
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) quit = true;
            emulator.HandleKeyEvent(e.type, e.key.keysym.sym);
        }
        if (window.UpdateTimerTick()) emulator.DecreaseDelayTimer();
        if (!emulator.IsWait()) {
            Chip8Opcode opcode = emulator.Fetch();
            emulator.Decode(opcode);
        }
        SDL_Delay(1);
        if (window.UpdateRenderTick()) window.UpdateScreen(emulator);
    }
}
