
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

#include "misc.h"

// #include "circular_buffer.h"

// FIFO BUFFER
typedef struct circular_buffer
{
    uintptr_t *buffer;
    int write_index;
    int read_index;
    int current_nr_of_items;
    int max_nr_of_items;
    uint32_t end_of_struct;
} circular_buffer_t;

int circular_buffer_get_internal_buffer_size(size_t max_nr_of_items)
{
    assert(sizeof(uintptr_t) == sizeof(void *));
    return max_nr_of_items * sizeof(uintptr_t);
}

return_status_t circular_buffer_init(void *circular_buffer_obj, size_t max_nr_of_items, void *buffer)
{
    circular_buffer_t *self = circular_buffer_obj;
    assert(max_nr_of_items > 2);

    printf("Init circular buffer\n");
    self->max_nr_of_items = max_nr_of_items;
    self->buffer = buffer;
    self->write_index = 0;
    self->read_index = 0;
    self->current_nr_of_items = 0;
    self->end_of_struct = 0x101FF101;
    return RET_OK;
}

void *circular_buffer_allocator(size_t max_nr_of_items)
{
    size_t allocation_size = sizeof(circular_buffer_t) + circular_buffer_get_internal_buffer_size(max_nr_of_items);
    printf("Size of circular buffer: %u, hex %x\n", sizeof(circular_buffer_t), sizeof(circular_buffer_t));
    printf("Total allocation: %u\n", allocation_size);

    return malloc(allocation_size);
}

void *circular_buffer_constructor(size_t max_nr_of_items)
{
    circular_buffer_t *object = circular_buffer_allocator(max_nr_of_items);
    if (object == NULL)
    {
        printf("Failed to allocate object\n");
        return NULL;
    }
    void *buffer = (char *)object + sizeof(circular_buffer_t);
    if (circular_buffer_init(object, max_nr_of_items, buffer) != RET_OK)
    {
        printf("Failed to initialize circular buffer object\n");
        return NULL;
    }

    return object;
}

return_status_t circular_buffer_destructor(void *circular_buffer_obj)
{
    circular_buffer_t *self = circular_buffer_obj;
    free(circular_buffer_obj);
    circular_buffer_obj = NULL;
    return RET_OK;
}

bool circular_buffer_is_full(void *circular_buffer_obj)
{
    circular_buffer_t *self = circular_buffer_obj;
    return self->current_nr_of_items == self->max_nr_of_items;
}

bool circular_buffer_is_empty(void *circular_buffer_obj)
{
    circular_buffer_t *self = circular_buffer_obj;
    return self->current_nr_of_items == 0;
}

return_status_t circular_buffer_push(void *circular_buffer_obj, void *item_pointer)
{
    circular_buffer_t *self = circular_buffer_obj;
    printf("Push %p to %p, items %i \n", item_pointer, &self->buffer[self->write_index], self->current_nr_of_items);

    self->buffer[self->write_index] = (uintptr_t)item_pointer;
    self->write_index = (self->write_index + 1) % self->max_nr_of_items;
    self->current_nr_of_items += 1;
    return RET_OK;
}

return_status_t circular_buffer_try_push(void *circular_buffer_obj, void *item_pointer)
{
    circular_buffer_t *self = circular_buffer_obj;
    if (circular_buffer_is_full(circular_buffer_obj))
    {
        printf("Buffer is full\n");
        return RET_FULL;
    }
    return circular_buffer_push(circular_buffer_obj, item_pointer);
}

return_status_t circular_buffer_pop(void *circular_buffer_obj, void **out_item_pointer)
{
    circular_buffer_t *self = circular_buffer_obj;
    if (out_item_pointer != NULL)
    {
        *out_item_pointer = (void *)self->buffer[self->read_index];
    }
    printf("Pop %p from %p, items: %i\n", *out_item_pointer, &self->buffer[self->read_index], self->current_nr_of_items);
    self->read_index = (self->read_index + 1) % self->max_nr_of_items;
    self->current_nr_of_items -= 1;
    return RET_OK;
}

return_status_t circular_buffer_try_pop(void *circular_buffer_obj, void **out_item_pointer)
{
    circular_buffer_t *self = circular_buffer_obj;
    if (circular_buffer_is_empty(circular_buffer_obj))
    {
        printf("No data in buffer\n");
        return RET_EMPTY;
    }
    return circular_buffer_pop(circular_buffer_obj, out_item_pointer);
}

return_status_t circular_buffer_print(void *circular_buffer_obj)
{
    circular_buffer_t *self = circular_buffer_obj;
    printf("Debug circular_buffer\n");
    printf("Mem start %p, size %i\n", self->buffer, self->max_nr_of_items);
    printf("Write %i Read %i\n", self->write_index, self->read_index);
    for (int i = 0; i < self->max_nr_of_items; i++)
    {
        void *address = &(self->buffer[i]);
        void *item = (void *)self->buffer[i];
        printf("%i : address: %p  Item: %p\n", i, address, item);
    }
    return RET_OK;
}

void test_stack()
{
    printf("---Test stack---");

    circular_buffer_t buffer;
    void *internal_buffer[5];
    if (circular_buffer_init(&buffer, 5, internal_buffer) != RET_OK)
    {
        printf("Failed to create buffer\n");
    }
    circular_buffer_push(&buffer, (void*)3);
    circular_buffer_print(&buffer);
}

void test_heap()
{
    printf("---Test heap---");

    circular_buffer_t *buffer = circular_buffer_constructor(5);
    if (buffer == NULL)
    {
        printf("Failed to create buffer\n");
    }
    circular_buffer_push(buffer, (void*)4);
    circular_buffer_print(buffer);
}

// buffer full
void test1()
{
    printf("---Test 1---");

    circular_buffer_t buffer;
    circular_buffer_t *buffer_ptr = &buffer;
    void *internal_buffer[5];
    if (circular_buffer_init(buffer_ptr, 5, NULL) != 0)
    {
        printf("Failed to create buffer\n");
    }

    void *j;
    circular_buffer_print(buffer_ptr);

    circular_buffer_try_push(buffer_ptr, (void *)1);
    circular_buffer_try_push(buffer_ptr, (void *)2);
    circular_buffer_try_push(buffer_ptr, (void *)3);
    circular_buffer_try_pop(buffer_ptr, &j);
    printf("j: %lli\n", j);
    circular_buffer_try_pop(buffer_ptr, &j);
    printf("j: %lli\n", j);
    circular_buffer_try_push(buffer_ptr, (void *)4);
    circular_buffer_try_push(buffer_ptr, (void *)5);
    circular_buffer_try_push(buffer_ptr, (void *)6);
    circular_buffer_try_push(buffer_ptr, (void *)7);
    circular_buffer_try_push(buffer_ptr, (void *)8);

    circular_buffer_print(buffer_ptr);
}

// Normal use
void test2()
{
    printf("---Test 2---");
    circular_buffer_t buffer;
    circular_buffer_t *buffer_ptr = &buffer;
    if (circular_buffer_init(buffer_ptr, 5, NULL) != RET_OK)
    {
        printf("Failed to create buffer\n");
    }

    void *j;
    circular_buffer_print(buffer_ptr);

    for (int i = 0; i < 30; i++)
    {
        circular_buffer_try_push(buffer_ptr, (void *)i);
        circular_buffer_try_pop(buffer_ptr, &j);
        printf("j: %lli\n", j);
    }
    circular_buffer_print(&buffer);
}

// pop empty
void test3()
{
    printf("---Test 3---");
    circular_buffer_t buffer;
    circular_buffer_t *buffer_ptr = &buffer;
    if (circular_buffer_init(buffer_ptr, 5, NULL) != RET_OK)
    {
        printf("Failed to create buffer\n");
    }

    void *j;
    circular_buffer_print(buffer_ptr);

    circular_buffer_try_push(buffer_ptr, (void *)1);
    circular_buffer_try_pop(buffer_ptr, &j);
    printf("j: %lli\n", j);
    circular_buffer_try_pop(buffer_ptr, &j);
    printf("j: %lli\n", j);

    circular_buffer_print(buffer_ptr);
}

int main()
{
    test_stack();

    test_heap();

    // test1();

    // test2();

    // test3();
}