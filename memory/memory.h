#ifndef MEMORY_H
#define MEMORY_H

#define HEAP_START 0x100000
#define HEAP_SIZE 0x100000

typedef struct block {
    unsigned int size;
    int free;
    struct block* next;
} block_t;

void memory_init(void);
void* malloc(unsigned int size);
void free(void* ptr);
void* memset(void* dest, int val, unsigned int count);
void* memcpy(void* dest, const void* src, unsigned int count);

#endif
