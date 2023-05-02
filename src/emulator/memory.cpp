#include "memory.h"

namespace emulator 
{
Memory::Memory()
    : memory{}
    , stack{}
    , registers{} 
    , special_registers({0, 0, 0})
{}
}