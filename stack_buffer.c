
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <limits.h>
#include <stdint.h>

#include "misc.h"
#include "stack_buffer.h"

typedef struct stack_buffer
{
    uintptr_t *buffer;
    size_t buffer_length;
    int index; // points to the next free memory
    size_t internal_buffer_memory_size;
    size_t total_memory_size;
    uint32_t end_of_struct;
} stack_buffer_t;

void *stack_buffer_allocator(size_t buffer_length)
{
    size_t internal_buffer_size = buffer_length * sizeof(uintptr_t);
    size_t allocation_size = sizeof(stack_buffer_t) + internal_buffer_size;

    stack_buffer_t *self = malloc(allocation_size);

    self->buffer = (uintptr_t*) ((char *)self + sizeof(stack_buffer_t));
    self->buffer_length = buffer_length;
    self->internal_buffer_memory_size = internal_buffer_size;
    self->total_memory_size = allocation_size;

    return self;
}

return_status_t stack_buffer_init(void *stack_buffer_obj, uintptr_t* buffer, size_t buffer_length)
{
    stack_buffer_t *self = stack_buffer_obj;
    self->index = 0;
    self->buffer = buffer;
    self->buffer_length = buffer_length;
    self->end_of_struct = 0x11000011; // memory pattern at the end for debugging 
    return RET_OK;
}

void *stack_buffer_constructor(size_t buffer_length)
{
    stack_buffer_t *self = stack_buffer_allocator(buffer_length);
    if (self == NULL)
        return NULL;
    if (stack_buffer_init(self, self->buffer, self->buffer_length) != RET_OK)
        return NULL;
    return self;
}

return_status_t stack_buffer_destructor(void *stack_buffer_obj)
{
    free(stack_buffer_obj);
    stack_buffer_obj = NULL;
    return RET_OK;
}

bool stack_buffer_is_full(void *stack_buffer_obj)
{
    stack_buffer_t *self = stack_buffer_obj;
    return self->index == self->buffer_length;
}

bool stack_buffer_is_empty(void *stack_buffer_obj)
{
    stack_buffer_t *self = stack_buffer_obj;
    return self->index == 0;
}

return_status_t stack_buffer_push(void *stack_buffer_obj, void *item_pointer)
{
    stack_buffer_t *self = stack_buffer_obj;
    self->buffer[self->index] = (uintptr_t)item_pointer;
    self->index++;
    return RET_OK;
}
 
return_status_t stack_buffer_pop(void *stack_buffer_obj, void **out_item_pointer)
{
    stack_buffer_t *self = stack_buffer_obj;
    if (out_item_pointer != NULL)
    {
        *out_item_pointer = (void *)self->buffer[self->index];
    }
    self->index--;
}

return_status_t stack_buffer_try_push(void *stack_buffer_obj, void *item_pointer)
{
    if (stack_buffer_is_full(stack_buffer_obj))
    {
        return RET_FULL;
    }
    return stack_buffer_push(stack_buffer_obj, item_pointer);
}

return_status_t stack_buffer_try_pop(void *stack_buffer_obj, void **out_item_pointer)
{
    if (stack_buffer_is_empty(stack_buffer_obj))
    {
        return RET_EMPTY;
    }
    return stack_buffer_pop(stack_buffer_obj, out_item_pointer);
}

return_status_t stack_buffer_print(void *stack_buffer_obj)
{
    stack_buffer_t *self = stack_buffer_obj;
    printf("Debug stack_buffer_buffer\n");
    printf("Buffer start %p, index %i, buffer_length %i\n", self->buffer, self->index, self->buffer_length);
    for (int i = 0; i < self->index; i++)
    {
        printf("Index %i :Item %llu Address %p\n", i, self->buffer[i], &self->buffer[i]);
    }
    return RET_OK;
}