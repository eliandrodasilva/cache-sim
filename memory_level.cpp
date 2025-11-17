#include "memory_level.h"

MemoryLevel::MemoryLevel(std::string name, int latency)
    : name(name), latency(latency), next_level(nullptr),
      read_accesses(0), write_accesses(0)
{
}

MemoryLevel::~MemoryLevel() {}

// Conecta a hierarquia
void MemoryLevel::set_next_level(MemoryLevel *next)
{
    this->next_level = next;
}

void MemoryLevel::displayProperties()
{
    std::cout << "  - Nome: " << this->name << std::endl;
    std::cout << "  - Latencia: " << this->latency << " ciclos" << std::endl;
}

void MemoryLevel::printStats()
{
    std::cout << "  - Acessos de Leitura: " << this->read_accesses << std::endl;
    std::cout << "  - Acessos de Escrita: " << this->write_accesses << std::endl;
    std::cout << "  - Total de Acessos: " << (this->read_accesses + this->write_accesses) << std::endl;
}