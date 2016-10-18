#include "CPU.h"

CPU::CPU()
{
    initialize();
}

void CPU::initialize()
{
    for (int i=0; i<32; i++)
        registers[i]=0;
    registers[28]= 268468224;
    registers[29]= 0x10010000+8192-4;
    programCounter= 0x00400000;
}
