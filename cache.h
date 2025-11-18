#ifndef CACHE_H
#define CACHE_H

#include "memory_level.h"
#include "cache_line.h"
#include <iostream>
#include <string>

class Cache : public MemoryLevel
{
private:
    int associativity;
    int num_sets;
    int line_size;
    int policy;

    int offset_bits;
    int index_bits;
    int tag_bits;

    CacheLine **sets;
    int global_time_counter;

    long hits;
    long misses;

    int get_index(int address);
    int get_tag(int address);
    int reconstruct_address(int tag, int index);
    CacheLine *find_line(int index, int tag);
    CacheLine *find_lru_line_to_replace(int index);
    int handle_hit(CacheLine *line, int type);
    int handle_miss(int address, int index, int tag, int type);

public:
    Cache(std::string name, int latency, int assoc, int num_sets, int line_size, int policy);
    ~Cache();

    int access(int address, int type);

    void displayProperties() override;
    void printStats() override;
};

#endif