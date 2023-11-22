#include "App.hpp"

chip8_app::chip8_app(const std::string& sdl_name, uint8_t width, uint8_t height)
    : emulator(width, height), window(sdl_name, width, height), quit(false)
{
}

bool chip8_app::load_rom(const std::string& rom_path)
{
    uint8_t rom_content[4096] = {0};
    uint16_t rom_content_len = 0;
    std::ifstream rom(rom_path, std::ios::in | std::ios::binary);
    if (!rom)
    {
        ERROR("Fail to read rom file");
        return false;
    }
    rom.seekg(0, std::ios::end);
    rom_content_len = rom.tellg();
    rom.seekg(0, std::ios::beg);

    rom.read((char*)rom_content + 0x200, rom_content_len);
    rom.close();

    emulator.set_rom(rom_content, rom_content_len + 0x200);

    return true;
}

void chip8_app::run()
{
    SDL_Event e;

    while (!quit)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT) quit = true;
            emulator.handle_key_event(e.type, e.key.keysym.sym);
        }
        if (window.update_timer_tick()) emulator.decrease_delay_timer();
        if (!emulator.is_wait())
        {
            chip8_opcode opcode = emulator.fetch();
            emulator.decode(opcode);
        }
        SDL_Delay(1);
        if (window.update_render_tick()) window.update_screen(emulator);
    }
}

chip8_app::~chip8_app() {}
