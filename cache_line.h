#ifndef CACHELINE_H
#define CACHELINE_H

struct CacheLine
{
    int valid;
    int dirty;
    int tag;
    int last_used_time; // Pra LRU

    CacheLine()
    {
        // (0 = falso, 1 = verdadeiro)
        valid = 0;
        dirty = 0;
        tag = 0;
        last_used_time = 0;
    }
};

#endif