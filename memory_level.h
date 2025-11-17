#ifndef MEMORYLEVEL_H
#define MEMORYLEVEL_H

#include <string>
#include <iostream>
#include <iomanip>
#include "Details.h"

class MemoryLevel
{
protected:
    std::string name;
    int latency;
    MemoryLevel *next_level;
    long read_accesses;
    long write_accesses;

public:
    MemoryLevel(std::string name, int latency);
    virtual ~MemoryLevel();
    virtual int access(int address, int type) = 0;
    void set_next_level(MemoryLevel *next);
    virtual void displayProperties();
    virtual void printStats();
};

#endif