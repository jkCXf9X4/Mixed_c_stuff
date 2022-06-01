
#pragma once

#include "stdint.h"

#include "misc.h"

// FIFO BUFFER


typedef struct circle_buffer
{
    uintptr_t *buffer;
    size_t buffer_length;
    int write_index;
    int read_index;
    size_t internal_buffer_memory_size;
    size_t total_memory_size;
    uint32_t end_of_struct;
} circle_buffer_t;

return_status_t circle_buffer_init(void *circle_buffer_obj, uintptr_t *internal_buffer, size_t buffer_length);

void *circle_buffer_constructor(size_t buffer_length);

return_status_t circle_buffer_destructor(void *circle_buffer_obj);

size_t circle_buffer_get_memory_size(void *circle_buffer_obj);

bool circle_buffer_is_full(void *circle_buffer_obj);

bool circle_buffer_is_empty(void *circle_buffer_obj);

return_status_t circle_buffer_read_next(void *circle_buffer_obj, void **out_item_pointer);

int circle_buffer_get_write_index(void *circle_buffer_obj);

int circle_buffer_get_read_index(void *circle_buffer_obj);

return_status_t circle_buffer_push(void *circle_buffer_obj, void *item_pointer);

return_status_t circle_buffer_try_push(void *circle_buffer_obj, void *item_pointer);

return_status_t circle_buffer_pop(void *circle_buffer_obj, void **out_item_pointer);

return_status_t circle_buffer_try_pop(void *circle_buffer_obj, void **out_item_pointer);

return_status_t circle_buffer_print(void *circle_buffer_obj);