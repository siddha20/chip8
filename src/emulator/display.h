#pragma once
#include <array>
#include "common.h"

namespace emulator
{
class Display
{
public:
    struct Position
    {
        size_t row;
        size_t col;
    };

    static constexpr size_t CHIP8_HEIGHT = 32;
    static constexpr size_t CHIP8_LENGTH = 64;

    static constexpr size_t ROWS = CHIP8_HEIGHT;
    static constexpr size_t COLS = CHIP8_LENGTH;
    static constexpr size_t SIZE = ROWS * COLS;

    static constexpr size_t SPRITE_COUNT = 16;
    static constexpr size_t SPRITE_SIZE = 5;
    static constexpr size_t SPRITE_ARRAY_SIZE = SPRITE_COUNT * SPRITE_SIZE;

    Display();

    std::array<u8, SIZE>::iterator get_display_iterator() { return display.begin(); }
    std::array<u8, SPRITE_ARRAY_SIZE>::iterator get_sprites_iterator() { return sprites.begin(); }
    u8 &operator [](const Position &position) { return get_pixel(position); }
    u8 operator [](const Position &position) const { return get_pixel(position); }
    void clear_display() { std::fill(display.begin(), display.end(), 0); }

private:
    std::array<u8, SIZE> display;
    std::array<u8, SPRITE_ARRAY_SIZE> sprites;

    void constexpr create_sprites();
    u8 &get_pixel(const Position &position) { return display[(position.row * COLS) + position.col]; }
    u8 get_pixel(const Position &position) const { return display[(position.row * COLS) + position.col]; }
};
}