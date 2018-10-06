#include "malloc.h"
#include "output.h"

#define FREE_SIZE 1024
#define NULL 0
#define USED_MAGIC 0x87654321

#define MALLOC_DBG_ON 1

#if MALLOC_DBG_ON
#define MALLOC_DBG printfmt
#else
#define MALLOC_DBG(...)
#endif

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
    MALLOC_DBG("free_space=%x first=%x first->start=%x\r\n", free_space, first, first->start);
}

void* malloc(unsigned int size)
{
    struct mHeader *ptr, *cur_ptr, *next_ptr;
    unsigned int realsize = (size / sizeof(struct mHeader)) + 1; // We need to add one for real header
    if (size % sizeof(struct mHeader) != 0)
        realsize += 1;
    MALLOC_DBG("size=%d, realsize=%d\r\n", size, realsize);
    ptr = &head;
    while (ptr->next) {
        cur_ptr = ptr->next;
        next_ptr = ptr->next->next;
        if (cur_ptr->size >= realsize) {
            if (cur_ptr->size != realsize) {
                ptr->next = &cur_ptr[realsize];
                ptr->next->size = cur_ptr->size - realsize;
                ptr->next->next = next_ptr;
            } else {  // cur_ptr->size == realsize
                ptr->next = next_ptr;
            }
            cur_ptr->size = realsize;
            cur_ptr->next = (struct mHeader *)USED_MAGIC;
            MALLOC_DBG("ptr=%x, ptr->size=%d\r\n", ptr, ptr->size);
            MALLOC_DBG("cur_ptr=%x, cur_ptr->next=%x, cur_ptr->size=%d\r\n", cur_ptr, cur_ptr->next, cur_ptr->size);
            MALLOC_DBG("ptr->next=%x, ptr->next->size=%d\r\n", ptr->next, ptr->next->size);
            return cur_ptr->start;
        }
        ptr = ptr->next;
    }
    return NULL;
}

void free(void* ptr)
{
    struct mHeader *next_ptr;
    struct mHeader *cur_ptr = &head;
    struct mHeader *info = (struct mHeader *)ptr;
    info -= 1;
    MALLOC_DBG("info=%x, info->next=%x, info->size=%d\r\n", info, info->next, info->size);
    if (info->next != (struct mHeader *)USED_MAGIC)
        return;
    while (cur_ptr->next < info) {
        cur_ptr = cur_ptr->next;
    }
    next_ptr = cur_ptr->next;
    /* Check whether need to combine */
    if (next_ptr == info + info->size) {
        info->next = next_ptr->next;
        info->size += next_ptr->size;
    } else {
        info->next = next_ptr;
    }
    cur_ptr->next = info;
}

void print_malloc(void)
{
    int i = 0;
    struct mHeader *ptr = (struct mHeader *)&head;
    MALLOC_DBG("-----MALLOC START-----\r\n");
    while (ptr) {
        MALLOC_DBG("%d: ptr=%x, ptr->next=%x, ptr->size=%d\r\n", i, ptr, ptr->next, ptr->size);
        i++;
        ptr = ptr->next;
    }
    MALLOC_DBG("-----MALLOC STOP-----\r\n");
}
