#ifndef MAINMEMORY_H
#define MAINMEMORY_H

#include "memory_level.h"
#include <iostream>

class MainMemory : public MemoryLevel
{
public:
    MainMemory(std::string name, int latency);

    int access(int address, int type);

    void displayProperties() override;
    void printStats() override;
};

#endif