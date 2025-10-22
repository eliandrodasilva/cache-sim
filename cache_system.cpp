#include "cache_system.hpp"
#include <iostream>

CacheSystem::CacheSystem(int line_size) : shared_line_size(line_size) {}

void CacheSystem::add_cache_level(const std::string &name, int latency, int associativity, int num_sets, WritePolicy policy)
{
    hierarchy.push_back(std::make_unique<Cache>(
        name, latency, associativity, num_sets, shared_line_size, policy));
}

void CacheSystem::add_main_memory(const std::string &name, int latency)
{
    hierarchy.push_back(std::make_unique<MainMemory>(name, latency));
}

void CacheSystem::displayHierarchy() const
{
    std::cout << "\n--- Hierarquia de Memoria Configurada ---" << std::endl;
    std::cout << "=========================================" << std::endl;
    if (hierarchy.empty())
    {
        std::cout << "Nenhum nivel de memoria foi configurado." << std::endl;
        return;
    }
    for (const auto &level : hierarchy)
    {
        level->displayProperties();
        std::cout << "-----------------------------------------" << std::endl;
    }
}
