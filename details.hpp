#ifndef DETAILS_H
#define DETAILS_H

// O tipo de acesso (leitura ou escrita)
enum AccessType
{
    READ,
    WRITE
};

// O padrao de acesso
enum AccessPattern
{
    SEQUENTIAL,
    RANDOM
};

// As politicas de escrita
enum WritePolicy
{
    WriteBack,
    WriteThrough
};

#endif