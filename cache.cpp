#include "Cache.hpp"
#include <iostream>

// Implementação do construtor
Cache::Cache(const std::string &name, int latency, int associativity, int num_sets, int line_size, WritePolicy policy)
    : MemoryLevel(name, latency),
      associativity(associativity),
      num_sets(num_sets),
      line_size(line_size),
      policy(policy)
{
  // Corpo do construtor (vazio neste caso)
}

// Implementação do método displayProperties
void Cache::displayProperties() const
{
  std::cout << "Nivel de Cache:" << std::endl;
  MemoryLevel::displayProperties(); // Chama o método da classe base
  std::cout << "  - Associatividade: " << associativity << " vias" << std::endl;
  std::cout << "  - Numero de Conjuntos: " << num_sets << std::endl;
  std::cout << "  - Tamanho da Linha: " << line_size << " bytes" << std::endl;
  std::cout << "  - Politica de Escrita: "
            << (policy == WritePolicy::WriteBack ? "Write-Back" : "Write-Through")
            << std::endl;
}

