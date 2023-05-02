#include <array>
#include <set>
#include "common.h"

namespace emulator
{
class Keyboard
{
public:
    static constexpr size_t KEYBOARD_SIZE = 16;

    Keyboard();

    void clear_status() { std::fill(key_status.begin(), key_status.end(), 0); }
    u8 get_key_press();

    u8 &operator [](size_t key) { return key_status[key]; }
    u8 operator [](size_t key) const { return key_status[key]; }

private:
    std::array<u8, KEYBOARD_SIZE> key_status;

    u8 &get_key_status(size_t key) { return key_status[key]; }
    u8 get_key_status(size_t key) const {return key_status[key]; }
};
}