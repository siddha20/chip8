#include "display.h"

namespace emulator
{
Display::Display()
    : display{}
    , sprites{}
{
    create_sprites(); 
}

void constexpr Display::create_sprites()
{
    // Digit 0
    sprites[0] = 0xF0;
    sprites[1] = 0x90;
    sprites[2] = 0x90;
    sprites[3] = 0x90;
    sprites[4] = 0xF0;

    // Digit 1
    sprites[5] = 0x20;
    sprites[6] = 0x60;
    sprites[7] = 0x20;
    sprites[8] = 0x20;
    sprites[9] = 0x70;

    // Digit 2
    sprites[10] = 0xF0;
    sprites[11] = 0x10;
    sprites[12] = 0xF0;
    sprites[13] = 0x80;
    sprites[14] = 0xF0;

    // Digit 3
    sprites[15] = 0xF0;
    sprites[16] = 0x10;
    sprites[17] = 0xF0;
    sprites[18] = 0x10;
    sprites[19] = 0xF0;

    // Digit 4
    sprites[20] = 0x90;
    sprites[21] = 0x90;
    sprites[22] = 0xF0;
    sprites[23] = 0x10;
    sprites[24] = 0x10;

    // Digit 5
    sprites[25] = 0xF0;
    sprites[26] = 0x80;
    sprites[27] = 0xF0;
    sprites[28] = 0x10;
    sprites[29] = 0xF0;

    // Digit 6
    sprites[30] = 0xF0;
    sprites[31] = 0x80;
    sprites[32] = 0xF0;
    sprites[33] = 0x90;
    sprites[34] = 0xF0;

    // Digit 7
    sprites[35] = 0xF0;
    sprites[36] = 0x10;
    sprites[37] = 0x20;
    sprites[38] = 0x40;
    sprites[39] = 0x40;

    // Digit 8
    sprites[40] = 0xF0;
    sprites[41] = 0x90;
    sprites[42] = 0xF0;
    sprites[43] = 0x90;
    sprites[44] = 0xF0;

    // Digit 9
    sprites[45] = 0xF0;
    sprites[46] = 0x90;
    sprites[47] = 0xF0;
    sprites[48] = 0x10;
    sprites[49] = 0xF0;

    // Digit A
    sprites[50] = 0xF0;
    sprites[51] = 0x90;
    sprites[52] = 0xF0;
    sprites[53] = 0x90;
    sprites[54] = 0x90;

    // Digit B
    sprites[55] = 0xE0;
    sprites[56] = 0x90;
    sprites[57] = 0xE0;
    sprites[58] = 0x90;
    sprites[59] = 0xE0;

    // Digit C
    sprites[60] = 0xF0;
    sprites[61] = 0x80;
    sprites[62] = 0x80;
    sprites[63] = 0x80;
    sprites[64] = 0xF0;

    // Digit D
    sprites[65] = 0xE0;
    sprites[66] = 0x90;
    sprites[67] = 0x90;
    sprites[68] = 0x90;
    sprites[69] = 0xE0;

    // Digit E
    sprites[70] = 0xF0;
    sprites[71] = 0x80;
    sprites[72] = 0xF0;
    sprites[73] = 0x80;
    sprites[74] = 0xF0;

    // Digit F
    sprites[75] = 0xF0;
    sprites[76] = 0x80;
    sprites[77] = 0xF0;
    sprites[78] = 0x80;
    sprites[79] = 0x80;
}
}