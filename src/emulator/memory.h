#pragma once
#include <cstddef>
#include <vector>
#include <array>
#include <memory>
#include "common.h"

namespace emulator 
{
class Memory 
{
struct SpecialRegisters {
    u16 I;
    u8 DT;
    u8 ST;
};

public:
    static constexpr size_t MEMORY_SIZE = 4096;
    static constexpr size_t STACK_SIZE  = 16;
    static constexpr size_t REGISTER_COUNT = 16;
    static constexpr size_t ROM_INDEX_START = 0x200;
    static constexpr size_t SPRITE_INDEX_START = 0x050;

    Memory();

    u8 &get_memory(size_t index) { return memory[index]; }
    u16 &get_stack(size_t index) { stack[index] &= 0x0FFF; return stack[index];}
    u8 &get_registers(size_t index) { return registers[index]; }
    void load_rom_to_mem(const std::vector<char> &ROM) { std::copy(ROM.begin(), ROM.end(), memory.begin() + ROM_INDEX_START); }
    auto get_memory_iterator() { return memory.begin(); }
    auto get_stack_iterator() { return stack.begin(); }
    auto get_registers_iterator() { return registers.begin(); }
    SpecialRegisters &get_special_registers() { return special_registers; }

private:
    std::array<u8, MEMORY_SIZE> memory;
    std::array<u16, STACK_SIZE> stack;
    std::array<u8, REGISTER_COUNT> registers;
    SpecialRegisters special_registers;
};
}