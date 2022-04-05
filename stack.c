
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include "linked_list.h"
#include "stack.h"

void *stack_allocator()
{
    return malloc(sizeof(stack_t));
}

int stack_init(void *stack_obj, size_t item_size, size_t start_num_of_items, size_t opt_max_num_of_items)
{
    stack_t *self = stack_obj;
    self->item_size = item_size;
    self->memory_start = malloc(item_size * start_num_of_items);
    self->head = self->memory_start;
    self->max_number_of_items = start_num_of_items;
    self->num_of_items = 0;
    if (opt_max_num_of_items == 0)
        opt_max_num_of_items = INT_MAX;
    self->allocation_max_num_of_items = opt_max_num_of_items;
    return 0;
}

void *stack_constructor(size_t item_size, size_t start_num_of_items, size_t max_num_of_items)
{
    stack_t *object = stack_allocator();
    if (object == NULL)
        return NULL;
    if (stack_init(object, item_size, start_num_of_items, max_num_of_items) != 0)
        return NULL;
    return object;
}

int stack_destructor(void *stack_obj, bool free_obj)
{
    stack_t *self = stack_obj;
    free(self->memory_start);
    if (free_obj)
        free(stack_obj);
    stack_obj = NULL;
    return 0;
}

void *stack_new(void *stack_obj)
{
    stack_t *self = stack_obj;
    if (self->num_of_items < self->max_number_of_items)
    {
        void *data = self->head;
        self->head += self->item_size;
        self->num_of_items++;
        return data;
    }
    else if (self->num_of_items == self->max_number_of_items && self->max_number_of_items < self->allocation_max_num_of_items)
    {
        size_t new_number_of_items = self->max_number_of_items * 2 < self->allocation_max_num_of_items ? self->max_number_of_items * 2 : self->allocation_max_num_of_items;
        self->memory_start = realloc(self->memory_start, new_number_of_items * self->item_size);
        if (self->memory_start == NULL)
        {
            // ERROR("Unable ro reallocate new data for stack")
            return NULL;
        }
        self->max_number_of_items = new_number_of_items;
        return stack_new(self);
    }
    else
    {
        // ERROR("Failed to allocate more memory from stack")
        return NULL;
    }
}

void *stack_push(void *stack_obj, void *item)
{
    stack_t *self = stack_obj;
    void * new_stack_pointer = stack_new(self);
    new_stack_pointer = item;
    return new_stack_pointer;
}

void *stack_get_data(void *stack_obj)
{
    stack_t *self = stack_obj;
    if (self->num_of_items > 0)
    {
        return self->head - self->item_size;
    }
    return NULL;
}

// 
int stack_pop(void *stack_obj)
{
    stack_t *self = stack_obj;
    if (self->num_of_items > 0)
    {
        self->head -= self->item_size;
        self->num_of_items--;
        return 0;
    }
    return -1;
}

int stack_print(void *stack_obj)
{
    stack_t *self = stack_obj;
    printf("Debug stack\n");
    printf("Mem start %p, head %p, item_size %lli\n", self->memory_start, self->head, self->item_size);
    printf(" Nr of items %lli, Max_nr_of_items %lli, Max_nr_of_items_allocated %lli\n", self->num_of_items, self->max_number_of_items, self->allocation_max_num_of_items);
    for (int i = 0; i < self->num_of_items; i++)
    {
        void *memory = self->memory_start + self->item_size * i;
        printf("%i : %p\n", i, memory);
    }
    return 0;
}