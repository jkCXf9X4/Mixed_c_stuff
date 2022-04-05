
#include <stdlib.h>

typedef struct free_stack
{
    void *stack;
    void *memory_start;
    void *memory_end;
    size_t memory_size;
    size_t num_of_items;
    size_t item_size;
}free_stack_t;


void *free_stack_allocator();

int free_stack_init(void *free_stack_obj, size_t item_size, size_t num_of_items);

void *free_stack_constructor(size_t item_size, size_t num_of_items);

int free_stack_destructor(void *free_stack_obj, bool free_obj);

void *free_stack_new(void * free_stack_obj);

int free_stack_free(void * free_stack_obj, void *item);