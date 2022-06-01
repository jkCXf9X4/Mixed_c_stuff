
#include <stdlib.h>

#include "stack_buffer.h"
#include "free_stack.h"

// allocator

typedef struct free_memory
{
    void * free_memory;
} free_memory_t;

void *free_stack_allocator()
{
    return malloc(sizeof(free_stack_t));
}

int free_stack_init(void *free_stack_obj, size_t item_size, size_t num_of_items)
{
    free_stack_t *self = free_stack_obj;
    self->stack = stack_constructor(sizeof(free_memory_t),num_of_items,num_of_items);

    self->memory_size = item_size * num_of_items;
    self->memory_start = malloc(self->memory_size);
    self->memory_end = self->memory_start + self->memory_size;

    // Fill stack
    for (void *i = self->memory_start; i < self->memory_end; i += item_size)
    {
        free_memory_t *free_memory = stack_new(self->stack);
        free_memory->free_memory = i;
    }
}
void *free_stack_constructor(size_t item_size, size_t num_of_items)
{
    free_stack_t *object = free_stack_allocator();
    if (object == NULL)
        return NULL;
    if (free_stack_init(object, item_size, num_of_items) != 0)
        return NULL;
    return object;
}

int free_stack_destructor(void *free_stack_obj, bool free_obj)
{
    free_stack_t *self = free_stack_obj;
    stack_destructor(self->stack, true);
    free(self->memory_start);
    if (free_obj)
        free(free_stack_obj);
    free_stack_obj = NULL;
}

void *free_stack_new(void *free_stack_obj)
{
    free_stack_t *self = free_stack_obj;
    free_memory_t *free_memory = stack_get_data(self->stack);
    void * temp = free_memory->free_memory;
    stack_pop(self->stack);
    return temp;
}

int free_stack_free(void *free_stack_obj, void *item)
{
    free_stack_t *self = free_stack_obj;
    free_memory_t *free_memory = stack_new(self->stack);
    free_memory->free_memory = item;
    return 0;
}