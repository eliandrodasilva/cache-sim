#ifndef MEMORYLEVEL_H
#define MEMORYLEVEL_H

#include <string>
#include <iostream>
#include "Details.hpp"

class MemoryLevel
{
protected:
    std::string name;
    int latency;
    MemoryLevel *next_level; // Ponteiro para o proximo nivel

public:
    MemoryLevel(std::string name, int latency)
    {
        this->name = name;
        this->latency = latency;
        this->next_level = NULL;
    }

    // Destrutor virtual (necessario para classes base, mas ainda não entendi)
    virtual ~MemoryLevel() {}

    // Funcao de acesso virtual
    virtual int access(int address, int type) = 0;

    // Conecta a hierarquia de memoria
    void set_next_level(MemoryLevel *next)
    {
        this->next_level = next;
    }

    virtual void displayProperties()
    {
        std::cout << "  - Nome: " << this->name << std::endl;
        std::cout << "  - Latencia: " << this->latency << " ciclos" << std::endl;
    }
};

#endif