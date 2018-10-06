#ifndef __MALLOC_H__
#define __MALLOC_H__

void malloc_init(void);
void* malloc(unsigned int size);
void free(void* ptr);

#endif /*__MALLOC_H_*/
