#include <iostream>
#include <stdlib.h>
#include "emulator.h"

int main(int argc, char *argv[])
{   
    #ifdef DEBUG
        std::cout << "DEBUG MODE" << std::endl;
    #endif

    std::string rom_path;
    if (argc != 2)
    {
        std::cerr << "Usage: chip8 <path to ROM>" << std::endl;
        exit(EXIT_FAILURE);
    }
    else rom_path = std::string(argv[1]);

    emulator::Runner r = emulator::Runner(rom_path);
    r.start();

    return EXIT_SUCCESS;
}