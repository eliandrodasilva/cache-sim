#ifndef CACHESYSTEM_HPP
#define CACHESYSTEM_HPP

#include <vector>
#include <memory>
#include <string>
#include "cache.hpp"       
#include "main_memory.hpp" 

class CacheSystem
{
private:
    int shared_line_size;
    std::vector<std::unique_ptr<MemoryLevel>> hierarchy;

public:
    CacheSystem(int line_size);

    void add_cache_level(const std::string &name, int latency, int associativity, int num_sets, WritePolicy policy);
    void add_main_memory(const std::string &name, int latency);
    void displayHierarchy() const;
};

#endif