#include "Processor.h"

Processor::Processor(MemoryLevel *l1)
    : l1_cache(l1), current_address(0), total_latency(0), total_accesses(0)
{
}

void Processor::generate_next_access(int pattern, int stride, double write_ratio, int buffer_size, int *out_address, int *out_type)
{

    if (pattern == SEQUENTIAL)
    {
        *out_address = this->current_address;
        this->current_address += stride;

        if (this->current_address >= buffer_size)
        {
            this->current_address = 0;
        }
    }
    else
    { // RANDOM
        if (buffer_size > 0)
        {
            *out_address = rand() % buffer_size;
        }
        else
        {
            *out_address = 0;
        }
    }

    if ((rand() % 100) < (write_ratio * 100))
    {
        *out_type = WRITE;
    }
    else
    {
        *out_type = READ;
    }
}

void Processor::run_simulation(int pattern, int num_accesses, int stride, double write_ratio, int buffer_size)
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
        generate_next_access(pattern, stride, write_ratio, buffer_size, &address, &type);

        int latency_for_this_access = this->l1_cache->access(address, type);

        this->total_latency += latency_for_this_access;
        this->total_accesses++;
    }
    std::cout << "Simulacao concluida." << std::endl;
}

void Processor::print_stats()
{
    if (this->total_accesses == 0)
    {
        std::cout << "Nenhuma simulacao executada." << std::endl;
        return;
    }

    double avg_latency = (double)this->total_latency / this->total_accesses;

    std::cout << "\n--- Estatisticas Globais da Simulacao ---" << std::endl;
    std::cout << "=========================================" << std::endl;
    std::cout << "Total de Acessos: " << this->total_accesses << std::endl;
    std::cout << "Tempo Total de Execucao (ciclos): " << this->total_latency << std::endl;
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Latencia Media (ciclos/acesso): " << avg_latency << std::endl;
}