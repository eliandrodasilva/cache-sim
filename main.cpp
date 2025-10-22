#include <iostream>
#include <limits>
#include <cstdio>
#include "cache_system.hpp"

int main()
{
    int total_levels;
    const int shared_line_size = 64;

    std::cout << "--- Configuracao do Simulador de Cache ---" << std::endl;
    std::cout << "Digite o numero total de niveis de memoria (ex: 3 para L1, L2, Main Memory): ";
    std::cin >> total_levels;

    while (std::cin.fail() || total_levels <= 1)
    {
        std::cout << "Entrada invalida. Por favor, digite um numero inteiro maior ou igual a 1: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> total_levels;
    }

    CacheSystem simulator(shared_line_size);
    std::cout << "\nConfigurando uma hierarquia com " << total_levels << " niveis." << std::endl;
    std::cout << "Tamanho da linha sera " << shared_line_size << " bytes para todas as caches." << std::endl;

    int num_cache_levels = total_levels - 1;

    for (int i = 0; i < num_cache_levels; ++i)
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
        std::cout << "Politica de Escrita (1 para Write-Through, 2 para Write-Back): ";
        std::cin >> policy_choice;

        WritePolicy policy = (policy_choice == 2) ? WritePolicy::WriteBack : WritePolicy::WriteThrough;

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

    return 0;
}
