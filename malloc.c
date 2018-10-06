#include "malloc.h"
#include "output.h"

#define FREE_SIZE 1024
#define NULL 0
#define USED_MAGIC 0x87654321

struct mHeader {
    struct mHeader *next;
    unsigned int size;
    unsigned char start[0];
};

static struct mHeader head;
static struct mHeader free_space[FREE_SIZE];

void malloc_init(void)
{
    struct mHeader *first = (struct mHeader *)free_space;
    head.next = first;
    head.size = 0;
    first->next = NULL;
    first->size = FREE_SIZE;
    printfmt("free_space=%x first=%x first->start=%x\r\n", free_space, first, first->start);
}

void* malloc(unsigned int size)
{
    struct mHeader *ptr, *cur_ptr;
    unsigned int realsize = (size / sizeof(struct mHeader)) + 1; // We need to add one for real header
    if (size % sizeof(struct mHeader) != 0)
        realsize += 1;
    ptr = &head;
    while (ptr->next) {
        cur_ptr = ptr->next;
        if (cur_ptr->size > realsize) {
            ptr->next = &cur_ptr[realsize];
            ptr->next->size = cur_ptr->size - realsize;
            cur_ptr->size = realsize;
            cur_ptr->next = (struct mHeader *)USED_MAGIC;
            return cur_ptr->start;
        }
        ptr = ptr->next;
    }
    return NULL;
}

void free(void* ptr)
{
}
