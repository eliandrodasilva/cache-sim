#include "cache_system.h"
#include <iostream>

CacheSystem::CacheSystem(int line_size)
    : shared_line_size(line_size)
{
}

CacheSystem::~CacheSystem()
{
    for (int i = 0; i < hierarchy.size(); i++)
    {
        delete hierarchy[i];
    }
}

void CacheSystem::add_cache_level(std::string name, int latency, int assoc, int num_sets, int policy)
{
    MemoryLevel *new_cache = new Cache(name, latency, assoc, num_sets, shared_line_size, policy);
    if (!hierarchy.empty())
    {
        hierarchy.back()->set_next_level(new_cache);
    }
    hierarchy.push_back(new_cache);
}

void CacheSystem::add_main_memory(std::string name, int latency)
{
    MemoryLevel *new_mem = new MainMemory(name, latency);
    if (!hierarchy.empty())
    {
        hierarchy.back()->set_next_level(new_mem);
    }
    hierarchy.push_back(new_mem);
}

MemoryLevel *CacheSystem::get_first_level()
{
    return hierarchy.empty() ? NULL : hierarchy[0];
}

void CacheSystem::displayHierarchy()
{
    std::cout << "\n--- Hierarquia de Memoria Configurada ---" << std::endl;
    std::cout << "=========================================" << std::endl;
    for (int i = 0; i < hierarchy.size(); i++)
    {
        hierarchy[i]->displayProperties();
        std::cout << "-----------------------------------------" << std::endl;
    }
}

void CacheSystem::printHierarchyStats()
{
    std::cout << "\n--- Estatisticas por Nivel da Hierarquia ---" << std::endl;
    std::cout << "============================================" << std::endl;
    for (int i = 0; i < hierarchy.size(); i++)
    {
        hierarchy[i]->printStats();
        if (i < hierarchy.size() - 1)
        {
            std::cout << "--------------------------------------------" << std::endl;
        }
    }
}