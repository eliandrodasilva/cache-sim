#ifndef CACHE_H
#define CACHE_H

#include "memory_level.hpp"
#include "cache_line.hpp"
#include <iostream>
#include <string>

// Funcao auxiliar para log2 (pra usar no bits)
// line_size = 64, C_log2(64) retorna 6 q eh o tamanho do offset em bits
int C_log2(int n)
{
    if (n <= 0)
        return 0;
    int count = 0;
    while (n > 1)
    {
        n = n >> 1; // Divisao por 2
        count++;
    }
    return count;
}

class Cache : public MemoryLevel
{
private:
    int associativity;
    int num_sets;
    int line_size;
    int policy;

    // Bits de endereco (32 bits q tá no dontpad, os outros tao la tbm)
    int offset_bits;
    int index_bits;
    int tag_bits;

    // Estrutura de dados: um array de arrays de CacheLine
    // É um array de ponteiros para arrays de CacheLine
    // Cada conjunto (set) eh um array de varias linhas (dependendo da associatividade)
    CacheLine **sets;

    // Contador global de tempo para LRU pra marcar o ultimo uso
    int global_time_counter;

    // --- Funcoes auxiliares privadas ---

    int get_index(int address)
    {
        // Mascara para pegar os bits do indice
        // Se offset_bits = 6 e index_bits = 4, então o índice são os bits 6–9 se eu nn tiver errado >w<
        int mask = (1 << this->index_bits) - 1;
        return (address >> this->offset_bits) & mask;
    }

    int get_tag(int address)
    {
        return address >> (this->offset_bits + this->index_bits);
    }

    // Isso é pra quando escreve de volta na memória inferior
    int reconstruct_address(int tag, int index)
    {
        return (tag << (this->index_bits + this->offset_bits)) | (index << this->offset_bits);
    }

    // Encontra uma linha pela tag. Se achar, atualiza seu tempo LRU.
    CacheLine *find_line(int index, int tag)
    {
        CacheLine *set = this->sets[index];
        for (int i = 0; i < this->associativity; i++)
        {
            if (set[i].valid == 1 && set[i].tag == tag)
            {
                // Atualiza o tempo de uso
                set[i].last_used_time = this->global_time_counter;
                return &set[i];
            }
        }
        return NULL;
    }

    // Encontra a linha para substituir (a menos usada)
    CacheLine *find_lru_line_to_replace(int index)
    {
        CacheLine *set = this->sets[index];

        // primeiro eu tento achar uma linha invalida (vazia)
        for (int i = 0; i < this->associativity; i++)
        {
            if (set[i].valid == 0)
            {
                return &set[i];
            }
        }

        // se naõ tem , acha a LRU (a com menor tempo) e como eu atualizo sempre q uso, a menor é a menos usada
        int min_time = set[0].last_used_time;
        int lru_index = 0;
        for (int i = 1; i < this->associativity; i++)
        {
            if (set[i].last_used_time < min_time)
            {
                min_time = set[i].last_used_time;
                lru_index = i;
            }
        }
        return &set[lru_index];
    }

    // Logica de Hit
    int handle_hit(CacheLine *line, int type)
    {
        int total_latency = this->latency; // Latencia de acerto

        if (type == WRITE)
        {
            if (this->policy == WriteBack)
            {
                line->dirty = 1; // Apenas marca como suja
            }
            else
            { // WriteThrough
                // Escreve no proximo nivel
                if (this->next_level != NULL)
                {
                    total_latency += this->next_level->access(0, WRITE);
                }
            }
        }
        return total_latency;
    }

    // Logica de Miss
    int handle_miss(int address, int index, int tag, int type)
    {
        int total_latency = this->latency; // Latencia do miss

        // 1. Busca no proximo nivel (sempre READ)
        if (this->next_level != NULL)
        {
            total_latency += this->next_level->access(address, READ);
        }

        // 2. Acha a linha para por o novo dado
        CacheLine *line_to_replace = find_lru_line_to_replace(index);

        // 3. Se a linha a ser trocada esta suja (e eh WriteBack), salva ela
        if (this->policy == WriteBack && line_to_replace->valid == 1 && line_to_replace->dirty == 1)
        {
            int evicted_address = reconstruct_address(line_to_replace->tag, index);
            if (this->next_level != NULL)
            {
                total_latency += this->next_level->access(evicted_address, WRITE);
            }
        }

        // 4. Coloca o novo dado na linha
        line_to_replace->valid = 1;
        line_to_replace->tag = tag;
        line_to_replace->last_used_time = this->global_time_counter;

        // 5. Lida com a escrita (se for um write miss)
        if (type == WRITE)
        {
            if (this->policy == WriteBack)
            {
                line_to_replace->dirty = 1;
            }
            else
            { // WriteThrough
                // Ja lemos, agora escrevemos
                if (this->next_level != NULL)
                {
                    total_latency += this->next_level->access(address, WRITE);
                }
                line_to_replace->dirty = 0;
            }
        }
        else
        {
            line_to_replace->dirty = 0; // Se for READ, a linha esta limpa
        }

        return total_latency;
    }

public:
    // Construtor
    Cache(std::string name, int latency, int assoc, int num_sets, int line_size, int policy)
        : MemoryLevel(name, latency)
    {

        this->associativity = assoc;
        this->num_sets = num_sets;
        this->line_size = line_size;
        this->policy = policy;
        this->global_time_counter = 0;

        // Calcula bits de endereco
        this->offset_bits = C_log2(line_size);
        this->index_bits = C_log2(num_sets);
        this->tag_bits = 32 - index_bits - offset_bits; // Assumindo 32 bits q eu anotei no dontpad

        // Alocacao de memoria
        this->sets = new CacheLine *[num_sets];
        for (int i = 0; i < num_sets; i++)
        {
            this->sets[i] = new CacheLine[associativity];
            // O construtor de CacheLine() zera os valores
        }
    }

    // Destrutor para liberar a memoria, tenho q ver como funciona isso direito
    ~Cache()
    {
        for (int i = 0; i < this->num_sets; i++)
        {
            delete[] this->sets[i]; // Deleta cada array de linhas
        }
        delete[] this->sets; // Deleta o array de ponteiros
    }

    // Funcao principal de acesso
    int access(int address, int type)
    {
        this->global_time_counter++; // O tempo passa a cada acesso

        int index = get_index(address);
        int tag = get_tag(address);

        CacheLine *line = find_line(index, tag);

        if (line != NULL)
        {
            return handle_hit(line, type); // Cache Hit
        }
        else
        {
            return handle_miss(address, index, tag, type); // Cache Miss
        }
    }

    void displayProperties()
    {
        std::cout << "Nivel de Cache:" << std::endl;
        MemoryLevel::displayProperties();
        std::cout << "  - Associatividade: " << this->associativity << " vias" << std::endl;
        std::cout << "  - Numero de Conjuntos: " << this->num_sets << std::endl;
        std::cout << "  - Tamanho da Linha: " << this->line_size << " bytes" << std::endl;
        std::cout << "  - Politica de Escrita: "
                  << (this->policy == WriteBack ? "Write-Back" : "Write-Through")
                  << std::endl;
    }
};

#endif