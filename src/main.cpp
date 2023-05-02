#include <iostream>
#include <fstream>
#include <array>
#include <typeinfo>
#include "emulator.h"

int main(int argc, char *argv[])
{

    // emulator::Memory m = emulator::Memory();
    // emulator::print_array(m.get_stack());
    // std::cout << std::endl;
    // m.get_stack()[0] = 255;
    // m.get_stack()[3] = 2587;
    // emulator::print_array_hex(m.get_stack());
    // std::cout << std::endl;
    // emulator::print_array(m.get_stack());
    // std::cout << std::endl;
    // std::cout << "test" << std::endl;


    // emulator::Processor p = emulator::Processor(); 
    // p.test1();
    // p.test2();

    // uint16_t a = 256;
    // uint8_t b = a;
    // std::cout << a << " "<< (int)b << std::endl;

    // emulator::Display d = emulator::Display();
    // d[{1, 2}] = 10;

        // emulator::Runner r = emulator::Runner("TETRIS");
    //    emulator::Runner r = emulator::Runner("CONNECT4");
    //    emulator::Runner r = emulator::Runner("BRIX");
    //    emulator::Runner r = emulator::Runner("PONG");
    emulator::Runner r = emulator::Runner("INVADERS");
    // emulator::Runner r = emulator::Runner("Chip8Logo.ch8");
    // emulator::Runner r = emulator::Runner("test_opcode.ch8");
    // emulator::Runner r = emulator::Runner("c8_test.c8");
    // emulator::Runner r = emulator::Runner("chip8-test-rom.ch8");
    // emulator::Runner r = emulator::Runner("chiptest.ch8");
    // emulator::Runner r = emulator::Runner("3-corax+.ch8");
    // emulator::Runner r = emulator::Runner("4-flags.ch8");
    // emulator::Runner r = emulator::Runner("5-quirks.ch8");
    // emulator::Runner r = emulator::Runner("6-keypad.ch8");
    
    r.start();



    return 0;
}