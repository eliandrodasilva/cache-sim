#ifndef CACHE_HPP
#define CACHE_HPP

#include "memory_level.hpp"

enum class WritePolicy
{
    WriteBack,
    WriteThrough
};

class Cache : public MemoryLevel
{
private:
    int associativity;
    int num_sets;
    int line_size;
    WritePolicy policy;

public:
    // Construtor
    Cache(const std::string &name, int latency, int associativity, int num_sets, int line_size, WritePolicy policy);

    // Sobrescreve o método da classe base
    void displayProperties() const override;
};

#endif
