#include <fstream>
#include <thread>

#include "Chip8App.hpp"
#include "Logger.hpp"

Chip8App::Chip8App(const std::string& appName, uint8_t winWidth,
                   uint8_t winHeight)
    : emulator_(winWidth, winHeight)
    , window_(appName, { winWidth, winHeight })
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
    while (!emulator_.IsQuit()) {
        auto eventOption = window_.PollEvent();
        if (eventOption.has_value()) {
            emulator_.HandleEvent(eventOption.value());
        }
        if (window_.UpdateTimerTick()) {
            emulator_.DecreaseDelayTimer();
        }
        if (!emulator_.IsWait()) {
            Chip8Opcode opcode = emulator_.Fetch();
            emulator_.Decode(opcode);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        if (window_.UpdateRenderTick()) {
            window_.UpdateScreen(emulator_);
        }
    }
}
