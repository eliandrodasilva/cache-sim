#ifndef MEMORYLEVEL_HPP
#define MEMORYLEVEL_HPP

#include <iostream>
#include <string>

class MemoryLevel
{
protected:
    std::string name;
    int latency;

public:
    MemoryLevel(const std::string &name, int latency) : name(name), latency(latency) {}
    virtual ~MemoryLevel() = default;

    virtual void displayProperties() const
    {
        std::cout << "  - Nome: " << name << std::endl;
        std::cout << "  - Latencia: " << latency << " ciclos" << std::endl;
    }
};

#endif