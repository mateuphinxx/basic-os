#include "memory.h"

static block_t* heap_start = 0;

void memory_init(void) asm("memory_init");
void memory_init(void) {
    heap_start = (block_t*)HEAP_START;
    heap_start->size = HEAP_SIZE - sizeof(block_t);
    heap_start->free = 1;
    heap_start->next = 0;
}

void* malloc(unsigned int size) asm("malloc");
void* malloc(unsigned int size) {
    block_t* current = heap_start;
    
    while (current) {
        if (current->free && current->size >= size) {
            if (current->size > size + sizeof(block_t)) {
                block_t* new_block = (block_t*)((char*)current + sizeof(block_t) + size);
                new_block->size = current->size - size - sizeof(block_t);
                new_block->free = 1;
                new_block->next = current->next;
                current->next = new_block;
                current->size = size;
            }
            current->free = 0;
            return (void*)((char*)current + sizeof(block_t));
        }
        current = current->next;
    }
    
    return 0;
}

void free(void* ptr) asm("free");
void free(void* ptr) {
    if (!ptr) return;
    
    block_t* block = (block_t*)((char*)ptr - sizeof(block_t));
    block->free = 1;
    
    block_t* current = heap_start;
    while (current) {
        if (current->free && current->next && current->next->free) {
            current->size += current->next->size + sizeof(block_t);
            current->next = current->next->next;
        }
        current = current->next;
    }
}

void* memset(void* dest, int val, unsigned int count) asm("memset");
void* memset(void* dest, int val, unsigned int count) {
    unsigned char* temp = (unsigned char*)dest;
    for (; count != 0; count--) {
        *temp++ = val;
    }
    return dest;
}

void* memcpy(void* dest, const void* src, unsigned int count) asm("memcpy");
void* memcpy(void* dest, const void* src, unsigned int count) {
    const unsigned char* sp = (const unsigned char*)src;
    unsigned char* dp = (unsigned char*)dest;
    for (; count != 0; count--) {
        *dp++ = *sp++;
    }
    return dest;
}
