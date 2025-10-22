#ifndef MAINMEMORY_HPP
#define MAINMEMORY_HPP

#include "memory_level.hpp"

class MainMemory : public MemoryLevel
{
public:
    MainMemory(const std::string &name, int latency) : MemoryLevel(name, latency) {}

    void displayProperties() const override
    {
        std::cout << "Memoria Principal:" << std::endl;
        MemoryLevel::displayProperties();
    }
};

#endif