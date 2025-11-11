// Processor.h
// Gera os acessos. Usa rand() do C.
#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "memory_level.hpp"
#include <iostream>
#include <iomanip>  // Para std::setprecision
#include <stdlib.h> // Para rand()

class Processor
{
private:
    MemoryLevel *l1_cache;
    int current_address;

    int total_latency;
    int total_accesses;

    // Gera o proximo acesso
    void generate_next_access(int pattern, int step, double write_ratio, int *out_address, int *out_type)
    {

        if (pattern == SEQUENTIAL)
        {
            *out_address = this->current_address;
            this->current_address += step;
        }
        else
        {
            *out_address = rand(); // rand() simples!
        }

        // Decide se eh leitura ou escrita
        if ((rand() % 100) < (write_ratio * 100))
        {
            *out_type = WRITE;
        }
        else
        {
            *out_type = READ;
        }
    }

public:
    Processor(MemoryLevel *l1)
    {
        this->l1_cache = l1;
        this->current_address = 0;
        this->total_latency = 0;
        this->total_accesses = 0;
    }

    void run_simulation(int pattern, int num_accesses, int step, double write_ratio)
    {
        if (this->l1_cache == NULL)
        {
            std::cout << "Erro: Processador nao conectado a memoria." << std::endl;
            return;
        }

        std::cout << "\nIniciando simulacao..." << std::endl;

        this->total_latency = 0;
        this->total_accesses = 0;
        this->current_address = 0;

        for (int i = 0; i < num_accesses; i++)
        {
            int address;
            int type;
            generate_next_access(pattern, step, write_ratio, &address, &type);

            // Chamada principal!
            int latency_for_this_access = this->l1_cache->access(address, type);

            this->total_latency += latency_for_this_access;
            this->total_accesses++;
        }
        std::cout << "Simulacao concluida." << std::endl;
    }

    void print_stats()
    {
        if (this->total_accesses == 0)
        {
            std::cout << "Nenhuma simulacao executada." << std::endl;
            return;
        }

        // "long double" eh mais C
        double avg_latency = (double)this->total_latency / this->total_accesses;

        std::cout << "\n--- Estatisticas da Simulacao ---" << std::endl;
        std::cout << "Total de Acessos: " << this->total_accesses << std::endl;
        std::cout << "Latencia Total (ciclos): " << this->total_latency << std::endl;
        std::cout << std::fixed << std::setprecision(2);
        std::cout << "Latencia Media (ciclos/acesso): " << avg_latency << std::endl;
    }
};

#endif