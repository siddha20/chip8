#include "keyboard.h"

namespace emulator
{
Keyboard::Keyboard()
    : key_status{}
{}

/* Returns first key found that is pressed or 0x10 if no keys are pressed. */
u8 Keyboard::get_key_press()
{
    for (u8 i = 0; i < KEYBOARD_SIZE; i++) if (key_status[i]) return i;
    return KEYBOARD_SIZE + 1;
}
}