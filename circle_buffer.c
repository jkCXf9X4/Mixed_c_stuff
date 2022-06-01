
#include <stdbool.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "circle_buffer.h"

return_status_t circle_buffer_init(void *circle_buffer_obj, uintptr_t *internal_buffer, size_t buffer_length)
{
    circle_buffer_t *self = circle_buffer_obj;
    assert(buffer_length > 2);

    self->write_index = 0;
    self->read_index = 0;
    self->buffer_length = buffer_length;
    self->buffer = internal_buffer;
    self->end_of_struct = 0x11000011; // memory pattern at the end for debugging 
    return RET_OK;
}

void *circle_buffer_allocator(size_t buffer_length)
{
    size_t internal_buffer_size = buffer_length * sizeof(uintptr_t);
    size_t allocation_size = sizeof(circle_buffer_t) + internal_buffer_size;

    circle_buffer_t *self = malloc(allocation_size);
    if (self == NULL)
        return NULL;
    self->buffer = (uintptr_t*) ((char *)self + sizeof(circle_buffer_t));
    self->buffer_length = buffer_length;
    self->internal_buffer_memory_size = internal_buffer_size;
    self->total_memory_size = allocation_size;
    return self;
}

void *circle_buffer_constructor(size_t buffer_length)
{
    circle_buffer_t *self = circle_buffer_allocator(buffer_length);
    if (self == NULL)
    {
        printf("Failed to allocate object\n");
        return NULL;
    }
    if (circle_buffer_init(self, self->buffer, self->buffer_length) != RET_OK)
    {
        printf("Failed to initialize circle buffer object\n");
        return NULL;
    }

    return self;
}

return_status_t circle_buffer_destructor(void *circle_buffer_obj)
{
    free(circle_buffer_obj);
    circle_buffer_obj = NULL;
    return RET_OK;
}

// only guaranteed to give correct answer if buffer is allocated using constructor
size_t circle_buffer_get_memory_size(void *circle_buffer_obj)
{
    circle_buffer_t *self = circle_buffer_obj;
    return self->total_memory_size;
}

int circle_buffer_get_write_index(void *circle_buffer_obj)
{
    circle_buffer_t *self = circle_buffer_obj;
    return self->write_index;
}

int circle_buffer_get_read_index(void *circle_buffer_obj)
{
    circle_buffer_t *self = circle_buffer_obj;
    return self->read_index;
}

bool circle_buffer_is_full(void *circle_buffer_obj)
{
    circle_buffer_t *self = circle_buffer_obj;
    int next = (self->write_index + 1) % self->buffer_length;
    return next == self->read_index;
}

bool circle_buffer_is_empty(void *circle_buffer_obj)
{
    circle_buffer_t *self = circle_buffer_obj;
    return self->read_index == self->write_index;
}

return_status_t circle_buffer_read_next(void *circle_buffer_obj, void **out_item_pointer)
{
    circle_buffer_t *self = circle_buffer_obj;
    *out_item_pointer = (void *)self->buffer[self->read_index];
}

return_status_t circle_buffer_push(void *circle_buffer_obj, void *item_pointer)
{
    circle_buffer_t *self = circle_buffer_obj;
    // printf("Push %p to %p, index %i\n", item_pointer, &self->buffer[self->write_index], self->write_index);
    self->buffer[self->write_index] = (uintptr_t)item_pointer;
    self->write_index = (self->write_index + 1) % self->buffer_length;
    return RET_OK;
}

return_status_t circle_buffer_pop(void *circle_buffer_obj, void **out_item_pointer)
{
    circle_buffer_t *self = circle_buffer_obj;
    if (out_item_pointer != NULL)
    {
        circle_buffer_read_next(circle_buffer_obj, out_item_pointer);
    }
    // printf("Pop %p from %p, Index: %i\n", out_item_pointer, &self->buffer[self->read_index], self->read_index);
    self->read_index = (self->read_index + 1) % self->buffer_length;
    return RET_OK;
}

return_status_t circle_buffer_try_push(void *circle_buffer_obj, void *item_pointer)
{
    if (circle_buffer_is_full(circle_buffer_obj))
    {
        return RET_FULL;
    }
    return circle_buffer_push(circle_buffer_obj, item_pointer);
}

return_status_t circle_buffer_try_pop(void *circle_buffer_obj, void **out_item_pointer)
{
    if (circle_buffer_is_empty(circle_buffer_obj))
    {
        return RET_EMPTY;
    }
    return circle_buffer_pop(circle_buffer_obj, out_item_pointer);
}

return_status_t circle_buffer_print(void *circle_buffer_obj)
{
    circle_buffer_t *self = circle_buffer_obj;
    printf("Debug circle_buffer\n");
    printf("Mem start %p, size %i\n", self->buffer, self->buffer_length);
    printf("Write %i Read %i\n", self->write_index, self->read_index);
    for (int i = 0; i < self->buffer_length; i++)
    {
        void *address = &(self->buffer[i]);
        void *item = (void *)self->buffer[i];
        printf("%i : address: %p  Item: %p\n", i, address, item);
    }
    return RET_OK;
}