#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "memory_level.h"
#include <iostream>
#include <iomanip>  // Para o std::setprecision
#include <stdlib.h> // Para o rand()

class Processor
{
private:
    MemoryLevel *l1_cache;
    int current_address;

    long total_latency;
    int total_accesses;

    void generate_next_access(int pattern, int stride, double write_ratio, int buffer_size, int *out_address, int *out_type);

public:
    Processor(MemoryLevel *l1);

    void run_simulation(int pattern, int num_accesses, int stride, double write_ratio, int buffer_size);
    void print_stats();
};

#endif