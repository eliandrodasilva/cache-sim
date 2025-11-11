#include <iostream>
#include <limits>
#include <string>
#include <stdlib.h> // Para srand() e rand()
#include <time.h>   // Para time()

#include "details.hpp"
#include "cache_system.hpp"
#include "processor.hpp"

// Funcao auxiliar para limpar o buffer de entrada (cin)
void clear_cin()
{
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int main()
{
    // Semeia o gerador aleatorio (importante!)
    srand(time(NULL));

    int total_levels;
    const int shared_line_size = 64;

    std::cout << "--- Configuracao do Simulador de Cache ---" << std::endl;
    std::cout << "Digite o numero total de niveis de memoria (ex: 3 para L1, L2, Main): ";
    std::cin >> total_levels;

    while (std::cin.fail() || total_levels < 1)
    {
        std::cout << "Entrada invalida. Digite um numero >= 1: ";
        clear_cin();
        std::cin >> total_levels;
    }

    CacheSystem simulator(shared_line_size);

    int num_cache_levels = total_levels - 1;

    for (int i = 0; i < num_cache_levels; i++)
    {
        std::cout << "\n--- Configurando Cache Nivel " << i + 1 << " (L" << i + 1 << ") ---" << std::endl;
        std::string name = "L" + std::to_string(i + 1) + " Cache";
        int latency, associativity, num_sets, policy_choice;

        std::cout << "Latencia (ciclos): ";
        std::cin >> latency;
        std::cout << "Associatividade (vias): ";
        std::cin >> associativity;
        std::cout << "Numero de conjuntos: ";
        std::cin >> num_sets;
        std::cout << "Politica de Escrita (1=Write-Through, 2=Write-Back): ";
        std::cin >> policy_choice;

        int policy = (policy_choice == 2) ? WriteBack : WriteThrough;
        simulator.add_cache_level(name, latency, associativity, num_sets, policy);
    }

    std::cout << "\n--- Configurando a Memoria Principal (Ultimo Nivel) ---" << std::endl;
    std::string mem_name;
    int mem_latency;
    std::cout << "Nome da Memoria Principal (ex: DDR4_RAM): ";
    std::cin >> mem_name;
    std::cout << "Latencia da Memoria Principal (ciclos): ";
    std::cin >> mem_latency;

    simulator.add_main_memory(mem_name, mem_latency);
    simulator.displayHierarchy();

    // --- 2. Configuracao da Simulacao ---

    int pattern_choice, num_accesses, step;
    double write_ratio;

    std::cout << "\n--- Configuracao da Simulacao ---" << std::endl;
    std::cout << "Padrao de acesso (1=Sequencial, 2=Aleatorio): ";
    std::cin >> pattern_choice;
    int pattern = (pattern_choice == 2) ? RANDOM : SEQUENTIAL;

    if (pattern == SEQUENTIAL)
    {
        std::cout << "Tamanho do passo sequencial (ex: 64): ";
        std::cin >> step;
    }
    else
    {
        step = 1; // Nao usado
    }

    std::cout << "Numero total de acessos: ";
    std::cin >> num_accesses;
    std::cout << "Proporcao de escritas (ex: 0.3 para 30%): ";
    std::cin >> write_ratio;

    // --- 3. Execucao ---

    Processor processor(simulator.get_first_level());
    processor.run_simulation(pattern, num_accesses, step, write_ratio);
    processor.print_stats();

    return 0;
}