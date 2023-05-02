#pragma once
#include <string>
#include "common.h"
#include "memory.h"
#include "display.h"
#include "keyboard.h"

namespace emulator
{
class Processor
{
using instruction = void (Processor::*)();

struct XYRegister
{
    size_t x_index;
    size_t y_index;
};

public:
    Display display;
    Keyboard keyboard;
    Memory memory;

    Processor();

    int get_program_counter() const { return static_cast<int>(program_counter); }
    int get_stack_pointer() const { return static_cast<int>(stack_pointer); }
    int get_last_instruction() const { return static_cast<int>(last_instruction); }
    const std::string &get_last_instruction_name() const { return last_instruction_name; }
    std::string debug_information() const;
    void step();
    void test1();

private:
    u16 program_counter;
    u8 stack_pointer;
    u16 last_instruction;
    instruction last_function;
    std::string last_instruction_name;
    bool pause_execution;

    XYRegister get_xy_from_instruction();
    instruction get_instruction();

    /* All chip8 instructions are below. */
    void SYS();
    void CLS();
    void RET();

    void JP1();
    void JP2();

    void CALL();

    void SE1();
    void SE2();

    void SNE1();
    void SNE2();

    void LD1();
    void LD2();
    void LD3();
    void LD4();
    void LD5();
    void LD6();
    void LD7();
    void LD8();
    void LD9();
    void LD10();
    void LD11();

    void ADD1();
    void ADD2();
    void ADD3();

    void OR();
    void AND();
    void XOR();
    void SUB();
    void SHR();
    void SUBN();
    void SHL();
    void RND();
    void DRW();
    void SKP();
    void SKNP();
};
}