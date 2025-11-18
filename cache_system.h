#ifndef CACHESYSTEM_H
#define CACHESYSTEM_H

#include <vector>
#include <string>
#include "Cache.h"
#include "main_memory.h"

class CacheSystem
{
private:
    int shared_line_size;
    std::vector<MemoryLevel *> hierarchy;

public:
    CacheSystem(int line_size);
    ~CacheSystem();

    void add_cache_level(std::string name, int latency, int assoc, int num_sets, int policy);
    void add_main_memory(std::string name, int latency);
    MemoryLevel *get_first_level();
    void displayHierarchy();
    void printHierarchyStats();
};

#endif