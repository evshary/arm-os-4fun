#ifndef __MALLOC_H__
#define __MALLOC_H__

void malloc_init(void);
void* malloc(unsigned int size);
void free(void* ptr);
void print_malloc(void);

#endif /*__MALLOC_H_*/
