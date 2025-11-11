#ifndef CACHESYSTEM_H
#define CACHESYSTEM_H

#include <vector>
#include <memory>
#include <string>
#include "cache.hpp"
#include "main_memory.hpp"

class CacheSystem
{
private:
    int shared_line_size;
    // vetor de ponteiros para os niveis de memoria
    std::vector<MemoryLevel *> hierarchy;

public:
    CacheSystem(int line_size)
    {
        this->shared_line_size = line_size;
    }

    // Destrutor para limpar a memoria (todos os niveis/ainda não entendo isso)
    ~CacheSystem()
    {
        for (int i = 0; i < hierarchy.size(); i++)
        {
            delete hierarchy[i]; // Deleta cada nivel
        }
    }

    void add_cache_level(std::string name, int latency, int assoc, int num_sets, int policy)
    {
        // Cria o objeto com new
        MemoryLevel *new_cache = new Cache(name, latency, assoc, num_sets, shared_line_size, policy);

        // Liga o nivel anterior a este
        if (!hierarchy.empty())
        {
            hierarchy.back()->set_next_level(new_cache);
        }

        hierarchy.push_back(new_cache);
    }

    void add_main_memory(std::string name, int latency)
    {
        MemoryLevel *new_mem = new MainMemory(name, latency);

        if (!hierarchy.empty())
        {
            hierarchy.back()->set_next_level(new_mem);
        }

        hierarchy.push_back(new_mem);
    }

    MemoryLevel *get_first_level()
    {
        return hierarchy.empty() ? NULL : hierarchy[0];
    }

    void displayHierarchy()
    {
        std::cout << "\n--- Hierarquia de Memoria Configurada ---" << std::endl;
        std::cout << "=========================================" << std::endl;
        for (int i = 0; i < hierarchy.size(); i++)
        {
            hierarchy[i]->displayProperties();
            std::cout << "-----------------------------------------" << std::endl;
        }
    }
};

#endif