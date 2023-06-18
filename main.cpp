#include <iostream>
#include "app.h"

int main(int argc, char* argv[])
{
    if (argc == 1)
    {
        printf("Usage: chip8 <rom>\n");
        return 1;
    }
    chip8_app app("CHIP8 Emulator", 64, 32);
    app.load_rom(argv[1]);
    app.run();
    return 0;
}
