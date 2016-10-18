#ifndef CPU_H
#define CPU_H

class CPU
{
public:
    CPU();
    int registers[32];
    unsigned int programCounter;
    void initialize();
};

#endif
