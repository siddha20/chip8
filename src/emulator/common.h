#pragma once
#include <algorithm>
#include <array>
#include <iostream>
#include <iterator>
#include <sstream>

namespace emulator 
{
using u8 = std::uint8_t;
using u16 = std::uint16_t;

/* Print array in decimal. */
template<typename T, size_t size>
inline void print_array(const std::array<T, size> &array)
{
    std::copy(array.begin(), array.end(), std::ostream_iterator<T>(std::cout, " "));
    std::cout << std::flush;
}

/* Print array in hex. */
template<typename T, size_t size>
inline void print_array_hex(const std::array<T, size> &array)
{
    std::ostringstream hex_digits;
    std::copy(array.begin(), array.end(), std::ostream_iterator<T>(hex_digits << std::hex, " "));
    std::cout << hex_digits.str() << std::flush;
}
}