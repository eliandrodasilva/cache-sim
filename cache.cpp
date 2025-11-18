#include "cache.h"

// 'static' torna ela visivel apenas dentro deste arquivo .cpp
static int C_log2(int n)
{
    if (n <= 0)
        return 0;
    int count = 0;
    while (n > 1)
    {
        n = n >> 1;
        count++;
    }
    return count;
}

Cache::Cache(std::string name, int latency, int assoc, int num_sets, int line_size, int policy)
    : MemoryLevel(name, latency), associativity(assoc), num_sets(num_sets), line_size(line_size), policy(policy), global_time_counter(0), hits(0), misses(0), offset_bits(C_log2(line_size)), index_bits(C_log2(num_sets)), tag_bits(32 - C_log2(num_sets) - C_log2(line_size)), sets(nullptr)
{
    sets = new CacheLine *[num_sets];
    for (int i = 0; i < num_sets; i++)
    {
        this->sets[i] = new CacheLine[associativity];
    }
}

Cache::~Cache()
{
    for (int i = 0; i < this->num_sets; i++)
    {
        delete[] this->sets[i];
    }
    delete[] this->sets;
}

int Cache::access(int address, int type)
{
    this->global_time_counter++;

    if (type == READ)
    {
        this->read_accesses++;
    }
    else
    {
        this->write_accesses++;
    }

    int index = get_index(address);
    int tag = get_tag(address);

    CacheLine *line = find_line(index, tag);

    if (line != NULL)
    {
        this->hits++;
        return handle_hit(line, type);
    }
    else
    {
        this->misses++;
        return handle_miss(address, index, tag, type);
    }
}

void Cache::displayProperties()
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

void Cache::printStats()
{
    std::cout << "--- Estatisticas para: " << this->name << " ---" << std::endl;

    double size_in_bytes = (double)(this->num_sets * this->associativity * this->line_size);
    double size_in_kbytes = size_in_bytes / 1024.0;
    std::cout << "  - Tamanho: " << std::fixed << std::setprecision(2) << size_in_kbytes << " KBytes" << std::endl;

    MemoryLevel::printStats();

    long total_accesses = this->read_accesses + this->write_accesses;
    std::cout << "  - Hits: " << this->hits << std::endl;
    std::cout << "  - Misses: " << this->misses << std::endl;

    double miss_rate = 0.0;
    if (total_accesses > 0)
    {
        miss_rate = (double)this->misses / (double)total_accesses;
    }
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "  - Taxa de Miss: " << (miss_rate * 100.0) << "%" << std::endl;
}

int Cache::get_index(int address)
{
    int mask = (1 << this->index_bits) - 1;
    return (address >> this->offset_bits) & mask;
}

int Cache::get_tag(int address)
{
    return address >> (this->offset_bits + this->index_bits);
}

int Cache::reconstruct_address(int tag, int index)
{
    return (tag << (this->index_bits + this->offset_bits)) | (index << this->offset_bits);
}

CacheLine *Cache::find_line(int index, int tag)
{
    CacheLine *set = this->sets[index];
    for (int i = 0; i < this->associativity; i++)
    {
        if (set[i].valid == 1 && set[i].tag == tag)
        {
            set[i].last_used_time = this->global_time_counter;
            return &set[i];
        }
    }
    return NULL; // MISS
}

CacheLine *Cache::find_lru_line_to_replace(int index)
{
    CacheLine *set = this->sets[index];
    for (int i = 0; i < this->associativity; i++)
    {
        if (set[i].valid == 0)
        {
            return &set[i];
        }
    }
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

int Cache::handle_hit(CacheLine *line, int type)
{
    int total_latency = this->latency;
    if (type == WRITE)
    {
        if (this->policy == WriteBack)
        {
            line->dirty = 1;
        }
        else
        { // WriteThrough
            if (this->next_level != NULL)
            {
                total_latency += this->next_level->access(0, WRITE);
            }
        }
    }
    return total_latency;
}

int Cache::handle_miss(int address, int index, int tag, int type)
{
    int total_latency = this->latency;
    if (this->next_level != NULL)
    {
        total_latency += this->next_level->access(address, READ);
    }
    CacheLine *line_to_replace = find_lru_line_to_replace(index);
    if (this->policy == WriteBack && line_to_replace->valid == 1 && line_to_replace->dirty == 1)
    {
        int evicted_address = reconstruct_address(line_to_replace->tag, index);
        if (this->next_level != NULL)
        {
            total_latency += this->next_level->access(evicted_address, WRITE);
        }
    }
    line_to_replace->valid = 1;
    line_to_replace->tag = tag;
    line_to_replace->last_used_time = this->global_time_counter;
    if (type == WRITE)
    {
        if (this->policy == WriteBack)
        {
            line_to_replace->dirty = 1;
        }
        else
        { // WriteThrough
            if (this->next_level != NULL)
            {
                total_latency += this->next_level->access(address, WRITE);
            }
            line_to_replace->dirty = 0;
        }
    }
    else
    {
        line_to_replace->dirty = 0;
    }
    return total_latency;
}