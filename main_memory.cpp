#include "main_memory.h"

MainMemory::MainMemory(std::string name, int latency) : MemoryLevel(name, latency)
{
}

int MainMemory::access(int address, int type)
{
    if (type == READ)
    {
        this->read_accesses++;
    }
    else
    {
        this->write_accesses++;
    }

    return this->latency;
}

void MainMemory::displayProperties()
{
    std::cout << "Memoria Principal:" << std::endl;
    MemoryLevel::displayProperties();
}

void MainMemory::printStats()
{
    std::cout << "--- Estatisticas para: " << this->name << " ---" << std::endl;
    MemoryLevel::printStats();
}