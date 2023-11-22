#include "Chip8App.hpp"
#include "Logger.hpp"

int main(int argc, char* argv[])
{
    if (argc == 1) {
        INFO("Usage: chip8 <rom>");
        return 1;
    }
    Chip8App app("CHIP8 Emulator", 64, 32);
    app.LoadRom(argv[1]);
    app.Run();
    return 0;
}
