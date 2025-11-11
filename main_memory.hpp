#ifndef MAINMEMORY_H
#define MAINMEMORY_H

#include "memory_level.hpp"
#include <iostream>

class MainMemory : public MemoryLevel
{
public:
    MainMemory(std::string name, int latency) : MemoryLevel(name, latency)
    {
        // O Construtuor da memory_level ja inicializa os atributos.
    }

    // Implementacao do acesso q tava na classe base
    int access(int address, int type)
    {
        return this->latency;
    }

    void displayProperties()
    {
        std::cout << "Memoria Principal:" << std::endl;
        // Chama a implementacao q deixei na classe base
        MemoryLevel::displayProperties();
    }
};

#endif