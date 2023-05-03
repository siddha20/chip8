#include <sstream>
#include "processor.h"

namespace emulator
{
Processor::Processor() 
    : memory()
    , display()
    , keyboard()
    , program_counter(memory.ROM_INDEX_START)
    , stack_pointer(0)
    , last_instruction(0)
    , pause_execution(0)
{
    // Insert sprites into memory.
    auto sprite_itr = display.get_sprites_iterator();
    std::copy( sprite_itr
             , sprite_itr + display.SPRITE_ARRAY_SIZE
             , memory.get_memory_iterator() + memory.SPRITE_INDEX_START );
}

void Processor::step()
{
    last_function = get_instruction();
    if (last_function) (this->*last_function)();
    if (!pause_execution &&
        last_function != &Processor::JP1 &&
        last_function != &Processor::JP2 &&
        last_function != &Processor::CALL &&
        last_function != &Processor::RET)
    {
        program_counter += 2;
    }
}

void Processor::decrement_timers()
{
    if (memory.get_special_registers().DT) --memory.get_special_registers().DT;
    if (memory.get_special_registers().ST) --memory.get_special_registers().ST;
}

std::string Processor::debug_information() const
{
    std::ostringstream debug_info;
    debug_info << std::hex;
    debug_info << "PC: " << get_program_counter() << std::endl;
    debug_info << "SP: " << get_stack_pointer() << std::endl;
    debug_info << "Last instr: " << last_instruction_name << std::endl;
    return debug_info.str();
}

Processor::XYRegister Processor::get_xy_from_instruction()
{
    return { static_cast<size_t>((last_instruction & 0x0F00) >> 8)
           , static_cast<size_t>((last_instruction & 0x00F0) >> 4) };
}

Processor::instruction Processor::get_instruction()
{
    u8 byte1 = memory.get_memory(program_counter);
    u8 byte2 = memory.get_memory(program_counter + 1);
    u16 instruction = (byte1 << 8) + byte2;
    last_instruction = instruction;

    // TODO: Switch to switch case instead.
    if (instruction == 0x00E0)
    {
        last_instruction_name = "CLS";
        return &Processor::CLS;
    }
    else if (instruction == 0x00EE)
    {
        last_instruction_name = "RET";
        return &Processor::RET;
    }
    else if ((instruction & 0xF000) >> 12 == 0x0)
    {
        last_instruction_name = "SYS";
        return &Processor::SYS;
    }
    else if ((instruction & 0xF000) >> 12 == 0x1)
    {
        last_instruction_name = "JP1";
        return &Processor::JP1;
    }
    else if ((instruction & 0xF000) >> 12 == 0x2)
    {
        last_instruction_name = "CALL";
        return &Processor::CALL;
    }
    else if ((instruction & 0xF000) >> 12 == 0x3)
    {
        last_instruction_name = "SE1";
        return &Processor::SE1;
    }
    else if ((instruction & 0xF000) >> 12 == 0x4)
    {
        last_instruction_name = "SNE1";
        return &Processor::SNE1;
    }
    else if ((instruction & 0xF000) >> 12 == 0x5 &&
             (instruction & 0x000F) >> 0 ==  0x0)
    {
        last_instruction_name = "SE2";
        return &Processor::SE2;
    }
    else if ((instruction & 0xF000) >> 12 == 0x6)
    {
        last_instruction_name = "LD1";
        return &Processor::LD1;
    }
    else if ((instruction & 0xF000) >> 12 == 0x7)
    {
        last_instruction_name = "ADD1";
        return &Processor::ADD1;
    }
    else if ((instruction & 0xF000) >> 12 == 0x8 &&
             (instruction & 0x000F) >> 0 ==  0x0)
    {
        last_instruction_name = "LD2";
        return &Processor::LD2;
    }
    else if ((instruction & 0xF000) >> 12 == 0x8 &&
             (instruction & 0x000F) >> 0  == 0x1)
    {
        last_instruction_name = "OR";
        return &Processor::OR;
    }
    else if ((instruction & 0xF000) >> 12 == 0x8 &&
             (instruction & 0x000F) >> 0  == 0x2)
    {
        last_instruction_name = "AND";
        return &Processor::AND;
    }
    else if ((instruction & 0xF000) >> 12 == 0x8 &&
             (instruction & 0x000F) >> 0  == 0x3)
    {
        last_instruction_name = "XOR";
        return &Processor::XOR;
    }
    else if ((instruction & 0xF000) >> 12 == 0x8 &&
             (instruction & 0x000F) >> 0  == 0x4)
    {
        last_instruction_name = "ADD2";
        return &Processor::ADD2;
    }
    else if ((instruction & 0xF000) >> 12 == 0x8 &&
             (instruction & 0x000F) >> 0  == 0x5)
    {
        last_instruction_name = "SUB";
        return &Processor::SUB;
    }
    else if ((instruction & 0xF000) >> 12 == 0x8 &&
             (instruction & 0x000F) >> 0  == 0x6)
    {
        last_instruction_name = "SHR";
        return &Processor::SHR;
    }
    else if ((instruction & 0xF000) >> 12 == 0x8 &&
             (instruction & 0x000F) >> 0  == 0x7)
    {
        last_instruction_name = "SUBN";
        return &Processor::SUBN;
    }
    else if ((instruction & 0xF000) >> 12 == 0x8 &&
             (instruction & 0x000F) >> 0  == 0xE)
    {
        last_instruction_name = "SHL";
        return &Processor::SHL;
    }
    else if ((instruction & 0xF000) >> 12 == 0x9 &&
             (instruction & 0x000F) >> 0  == 0x0)
    {
        last_instruction_name = "SNE2";
        return &Processor::SNE2;
    }
    else if ((instruction & 0xF000) >> 12 == 0xA)
    {
        last_instruction_name = "LD3";
        return &Processor::LD3;
    }
    else if ((instruction & 0xF000) >> 12 == 0xB)
    {
        last_instruction_name = "JP2";
        return &Processor::JP2;
    }
    else if ((instruction & 0xF000) >> 12 == 0xC)
    {
        last_instruction_name = "RND";
        return &Processor::RND;
    }
    else if ((instruction & 0xF000) >> 12 == 0xD)
    {
        last_instruction_name = "DRW";
        return &Processor::DRW;
    }
    else if ((instruction & 0xF000) >> 12 == 0xE &&
             (instruction & 0x00F0) >> 4  == 0x9 &&
             (instruction & 0x000F) >> 0  == 0xE)
    {
        last_instruction_name = "SKP";
        return &Processor::SKP;
    }
    else if ((instruction & 0xF000) >> 12 == 0xE &&
             (instruction & 0x00F0) >> 4  == 0xA &&
             (instruction & 0x000F) >> 0  == 0x1)
    {
        last_instruction_name = "SKNP";
        return &Processor::SKNP;
    }
    else if ((instruction & 0xF000) >> 12 == 0xF &&
             (instruction & 0x00F0) >> 4  == 0x0 &&
             (instruction & 0x000F) >> 0  == 0x7)
    {
        last_instruction_name = "LD4";
        return &Processor::LD4;
    }
    else if ((instruction & 0xF000) >> 12 == 0xF &&
             (instruction & 0x00F0) >> 4  == 0x0 &&
             (instruction & 0x000F) >> 0  == 0xA)
    {
        last_instruction_name = "LD5";
        return &Processor::LD5;
    }
    else if ((instruction & 0xF000) >> 12 == 0xF &&
             (instruction & 0x00F0) >> 4  == 0x1 &&
             (instruction & 0x000F) >> 0  == 0x5)
    {
        last_instruction_name = "LD6";
        return &Processor::LD6;
    }
    else if ((instruction & 0xF000) >> 12 == 0xF &&
             (instruction & 0x00F0) >> 4  == 0x1 &&
             (instruction & 0x000F) >> 0  == 0x8)
    {
        last_instruction_name = "LD7";
        return &Processor::LD7;
    }
    else if ((instruction & 0xF000) >> 12 == 0xF &&
             (instruction & 0x00F0) >> 4  == 0x1 &&
             (instruction & 0x000F) >> 0  == 0xE)
    {
        last_instruction_name = "ADD3";
        return &Processor::ADD3;
    }
    else if ((instruction & 0xF000) >> 12 == 0xF &&
             (instruction & 0x00F0) >> 4  == 0x2 &&
             (instruction & 0x000F) >> 0  == 0x9)
    {
        last_instruction_name = "LD8";
        return &Processor::LD8;
    }
    else if ((instruction & 0xF000) >> 12 == 0xF &&
             (instruction & 0x00F0) >> 4  == 0x3 &&
             (instruction & 0x000F) >> 0  == 0x3)
    {
        last_instruction_name = "LD9";
        return &Processor::LD9;
    }
    else if ((instruction & 0xF000) >> 12 == 0xF &&
             (instruction & 0x00F0) >> 4  == 0x5 &&
             (instruction & 0x000F) >> 0  == 0x5)
    {
        last_instruction_name = "LD10";
        return &Processor::LD10;
    }
    else if ((instruction & 0xF000) >> 12 == 0xF &&
             (instruction & 0x00F0) >> 4  == 0x6 &&
             (instruction & 0x000F) >> 0  == 0x5)
    {
        last_instruction_name = "LD11";
        return &Processor::LD11;
    }
    else 
    {
        last_instruction_name = "NONE";
        std::cerr << "Unknown instruction encountered." << std::endl;
        return nullptr;
    }
}

// 0nnn
void Processor::SYS()
{
    program_counter = last_instruction & 0x0FFF;
}

// 00E0
void Processor::CLS()
{
    display.clear_display();
}

// 00EE
void Processor::RET()
{
    program_counter = memory.get_stack(stack_pointer == 0 ? 0 : stack_pointer--);
}

void Processor::JP1()
{
    program_counter = last_instruction & 0x0FFF;
}

void Processor::JP2()
{
    program_counter = memory.get_registers(0) + (last_instruction & 0x0FFF);
}

void Processor::CALL()
{
    memory.get_stack(++stack_pointer) = (program_counter & 0x0FFF) + 2;
    program_counter = last_instruction & 0x0FFF;
}

void Processor::SE1()
{
    auto [x, _] = get_xy_from_instruction();
    if (memory.get_registers(x) == (last_instruction & 0x00FF)) program_counter += 2;
}

void Processor::SE2()
{
    auto [x, y] = get_xy_from_instruction();
    if (memory.get_registers(x) == memory.get_registers(y)) program_counter += 2;
}

void Processor::SNE1()
{
    auto [x, _] = get_xy_from_instruction();
    if (memory.get_registers(x) != (last_instruction & 0x00FF)) program_counter += 2;
}

void Processor::SNE2()
{
    auto [x, y] = get_xy_from_instruction();
    if (memory.get_registers(x) != memory.get_registers(y)) program_counter += 2;
}

void Processor::LD1()
{
    auto [x, _] = get_xy_from_instruction();
    memory.get_registers(x) = last_instruction & 0x00FF;
}

void Processor::LD2()
{
    auto [x, y] = get_xy_from_instruction();
    memory.get_registers(x) = memory.get_registers(y);
}

void Processor::LD3()
{
    memory.get_special_registers().I = last_instruction & 0x0FFF;
}

void Processor::LD4()
{
    auto [x, _] = get_xy_from_instruction();
    memory.get_registers(x) = memory.get_special_registers().DT;
}

void Processor::LD5()
{
    static bool press_down;
    static u8 prev_key_press;
    u8 key = keyboard.get_key_press();
    if (!pause_execution && key > keyboard.KEYBOARD_SIZE) pause_execution = true;

    if (pause_execution && key < keyboard.KEYBOARD_SIZE)
    {
        press_down = true;
        prev_key_press = key;
    }
    if (press_down && pause_execution && key > keyboard.KEYBOARD_SIZE)
    {
        pause_execution = false;
        press_down = false;
        auto [x, _] = get_xy_from_instruction();
        memory.get_registers(x) = prev_key_press;
    }
}

void Processor::LD6()
{
    auto [x, _] = get_xy_from_instruction();
    memory.get_special_registers().DT = memory.get_registers(x);
}

void Processor::LD7()
{
    auto [x, _] = get_xy_from_instruction();
    memory.get_special_registers().ST = memory.get_registers(x);
}

void Processor::LD8()
{
    auto [x, _] = get_xy_from_instruction();
    u8 sprite = memory.get_registers(x);
    u16 sprite_location = (sprite * display.SPRITE_SIZE) + memory.SPRITE_INDEX_START;
    memory.get_special_registers().I = sprite_location;
}

void Processor::LD9()
{
    auto [x, _] = get_xy_from_instruction();
    u16 temp = memory.get_registers(x);
    u16 I = memory.get_special_registers().I;
    memory.get_memory(I+2) = temp % 10;
    temp /= 10;
    memory.get_memory(I+1) = temp % 10;
    temp /= 10;
    memory.get_memory(I) = temp % 10;
}

void Processor::LD10()
{
    auto [x, _] = get_xy_from_instruction();
    auto registers_itr = memory.get_registers_iterator();
    u16 I = memory.get_special_registers().I;
    std::copy(registers_itr, registers_itr + (x + 1), memory.get_memory_iterator() + I);

    // Do I need this?
    // memory.get_special_registers().I += (x + 1);
}

void Processor::LD11()
{
    auto [x, _] = get_xy_from_instruction();
    auto memory_itr = memory.get_memory_iterator();
    u16 I = memory.get_special_registers().I;
    std::copy(memory_itr + I, memory_itr + I + (x + 1), memory.get_registers_iterator());
    
    // Do I need this?
    // memory.get_special_registers().I += (x + 1);
}

void Processor::ADD1()
{
    auto [x, _] = get_xy_from_instruction();
    memory.get_registers(x) += last_instruction & 0x00FF;
}

void Processor::ADD2()
{
    auto [x, y] = get_xy_from_instruction();
    u16 sum = memory.get_registers(x) + memory.get_registers(y);
    memory.get_registers(x) = sum;
    memory.get_registers(0xF) = sum > 255 ?  1 : 0;
}

void Processor::ADD3()
{
    auto [x, _] = get_xy_from_instruction();
    memory.get_special_registers().I += memory.get_registers(x);
}

void Processor::OR()
{
    auto [x, y] = get_xy_from_instruction();
    memory.get_registers(x) |= memory.get_registers(y);

    // Do I need this?
    memory.get_registers(0xF) = 0;
}

void Processor::AND()
{
    auto [x, y] = get_xy_from_instruction();
    memory.get_registers(x) &= memory.get_registers(y);

    // Do I need this?
    memory.get_registers(0xF) = 0;
}

void Processor::XOR()
{
    auto [x, y] = get_xy_from_instruction();
    memory.get_registers(x) ^= memory.get_registers(y);

    // Do I need this?
    memory.get_registers(0xF) = 0;
}

void Processor::SUB()
{
    auto [x, y] = get_xy_from_instruction();
    if (x == 0xF) std::cout << "WHYYY" << std::endl;
    if (y == 0xF) std::cout << "WHYYY2" << std::endl;
    // u8 result = memory.get_registers(x) - memory.get_registers(y);
    memory.get_registers(0xF) = memory.get_registers(x) > memory.get_registers(y) ? 1 : 0;
    memory.get_registers(x) -= memory.get_registers(y);

    // memory.get_registers(x) = result;

}

void Processor::SHR()
{
    auto [x, _] = get_xy_from_instruction();
    memory.get_registers(0xF) =  memory.get_registers(x) & 1 ? 1 : 0;
    memory.get_registers(x) >>= 1;

    // Do I do this?
    // auto [x, y] = get_xy_from_instruction();
    // memory.get_registers(x) = memory.get_registers(y);
    // memory.get_registers(0xF) =  memory.get_registers(x) & 1 ? 1 : 0;
    // memory.get_registers(x) >>= 1;


}

void Processor::SUBN()
{
    auto [x, y] = get_xy_from_instruction();
    // u8 result = memory.get_registers(y) - memory.get_registers(x);
    // memory.get_registers(x) = memory.get_registers(y) - memory.get_registers(x);
    memory.get_registers(0xF) = memory.get_registers(y) > memory.get_registers(x) ?  1 : 0;
    // memory.get_registers(x) = result;
    memory.get_registers(x) = memory.get_registers(y) - memory.get_registers(x);
}

void Processor::SHL()
{
    auto [x, _] = get_xy_from_instruction();
    memory.get_registers(0xF) =  memory.get_registers(x) >> 7 ? 1 : 0;
    memory.get_registers(x) <<= 1;

    // Do I do this?
    // auto [x, y] = get_xy_from_instruction();
    // memory.get_registers(x) = memory.get_registers(y);
    // memory.get_registers(0xF) =  memory.get_registers(x) >> 7 ? 1 : 0;
    // memory.get_registers(x) <<= 1;
}

void Processor::RND()
{
    auto [x, _] = get_xy_from_instruction();
    u8 rand = std::rand() % 0xFF;
    memory.get_registers(x) = rand & (last_instruction & 0x00FF);
}

void Processor::DRW()
{
    auto [x, y] = get_xy_from_instruction();
    u16 I = memory.get_special_registers().I;
    u8 n = last_instruction & 0x000F;
    u8 Vx = memory.get_registers(x);
    u8 Vy = memory.get_registers(y);
    memory.get_registers(0xF) = 0;

    /* Debug */
    // u16 I = memory.SPRITE_INDEX_START + 50;
    // u8 n = 5;
    // u8 Vx = 20;
    // u8 Vy = 10;
    // u8 collision = 0;

    for (u8 i = 0; i < n; i++)
    {
        u8 sprite = memory.get_memory(I + i);
        for (u8 j = 0; j < 8; j++)
        {
            u8 new_pixel = (sprite >> (7 - j)) & 1;
            size_t row = (Vx + j) % display.CHIP8_LENGTH;
            size_t col = (Vy + i) % display.CHIP8_HEIGHT;
            // memory.get_registers(0xF) |= (display[{col, row}] != new_pixel) && (display[{col, row}] == 1); 
            memory.get_registers(0xF) |= new_pixel & display[{col, row}]; 
     
            display[{col, row}] ^= new_pixel;
        }
    }
}

void Processor::SKP()
{
    auto [x, _] = get_xy_from_instruction();
    if (keyboard[memory.get_registers(x)]) program_counter += 2;
}

void Processor::SKNP()
{
    auto [x, _] = get_xy_from_instruction();
    if (!keyboard[memory.get_registers(x)]) program_counter += 2;
}

void Processor::test1()
{
    instruction func = nullptr;

    memory.get_memory(0) = 0x00;
    memory.get_memory(1) = 0xE0;
    func = get_instruction();
    if (func) (this->*func)();
    std::cout << program_counter << " : " << last_instruction << " : " << last_instruction_name << std::endl;

    memory.get_memory(2) = 0x00;
    memory.get_memory(3) = 0xEE;
    func = get_instruction();
    if (func) (this->*func)();
    std::cout << program_counter << " : " << last_instruction << " : " << last_instruction_name << std::endl;

    memory.get_memory(4) = 0x1A;
    memory.get_memory(5) = 0xBC;
    func = get_instruction();
    if (func) (this->*func)();
    std::cout << program_counter << " : " << last_instruction << " : " << last_instruction_name << std::endl;

    memory.get_memory(6) = 0x2A;
    memory.get_memory(7) = 0xBC;
    func = get_instruction();
    if (func) (this->*func)();
    std::cout << program_counter << " : " << last_instruction << " : " << last_instruction_name << std::endl;

    memory.get_memory(8) = 0x5A;
    memory.get_memory(9) = 0xB0;
    func = get_instruction();
    if (func) (this->*func)();
    std::cout << program_counter << " : " << last_instruction << " : " << last_instruction_name << std::endl;

    memory.get_memory(10) = 0x5A;
    memory.get_memory(11) = 0xB0;
    func = get_instruction();
    if (func) (this->*func)();
    std::cout << program_counter << " : " << last_instruction << " : " << last_instruction_name << std::endl;

    memory.get_memory(12) = 0x6A;
    memory.get_memory(13) = 0xBC;
    func = get_instruction();
    if (func) (this->*func)();
    std::cout << program_counter << " : " << last_instruction << " : " << last_instruction_name << std::endl;

    memory.get_memory(14) = 0x7A;
    memory.get_memory(15) = 0xBC;
    func = get_instruction();
    if (func) (this->*func)();
    std::cout << program_counter << " : " << last_instruction << " : " << last_instruction_name << std::endl;

    memory.get_memory(16) = 0x8A;
    memory.get_memory(17) = 0xB0;
    func = get_instruction();
    if (func) (this->*func)();
    std::cout << program_counter << " : " << last_instruction << " : " << last_instruction_name << std::endl;

    memory.get_memory(18) = 0x8A;
    memory.get_memory(19) = 0xB1;
    func = get_instruction();
    if (func) (this->*func)();
    std::cout << program_counter << " : " << last_instruction << " : " << last_instruction_name << std::endl;

    memory.get_memory(20) = 0x8A;
    memory.get_memory(21) = 0xB2;
    func = get_instruction();
    if (func) (this->*func)();
    std::cout << program_counter << " : " << last_instruction << " : " << last_instruction_name << std::endl;

    memory.get_memory(22) = 0x8A;
    memory.get_memory(23) = 0xB3;
    func = get_instruction();
    if (func) (this->*func)();
    std::cout << program_counter << " : " << last_instruction << " : " << last_instruction_name << std::endl;
}
}