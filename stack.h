
#include <stdlib.h>
#include <stdbool.h>

typedef struct stack
{
    void *memory_start;
    void *head; // points to the next free memory
    size_t item_size;
    size_t num_of_items;
    size_t max_number_of_items; // max for the current allocation
    size_t allocation_max_num_of_items; // Will not allocate larger than this
} stack_t;

void *stack_allocator();

int stack_init(void *stack_obj, size_t item_size, size_t start_num_of_items, size_t opt_max_num_of_items);

void *stack_constructor(size_t item_size, size_t start_num_of_items, size_t max_num_of_items);

int stack_destructor(void *stack_obj, bool free_obj);

void *stack_new(void *stack_obj);

void *stack_push(void *stack_obj, void *item);

void *stack_get_data(void *stack_obj);

int stack_pop(void *stack_obj);

int stack_print(void *stack_obj);