#include "malloc.h"

#define FREE_SIZE 2048

static unsigned char free_space[FREE_SIZE];
static unsigned char *free_ptr;

void malloc_init(void)
{
    free_ptr = free_space;
}

void* malloc(unsigned int size)
{
    return 0;
}

void free(void* ptr)
{
}
