
#include <stdbool.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>

#include "circle_buffer.h"

// void test_stack()
// {
//     printf("---Test stack---");

//     circle_buffer_t buffer;
//     void *internal_buffer[5];
//     if (circle_buffer_init(&buffer, 5, internal_buffer) != RET_OK)
//     {
//         printf("Failed to create buffer\n");
//     }
//     circle_buffer_push(&buffer, (void*)3);
//     circle_buffer_print(&buffer);
// }

void test_heap()
{
    printf("---Test heap---\n");

    void *buffer = circle_buffer_constructor(3);
    if (buffer == NULL)
    {
        printf("Failed to create buffer\n");
    }
    circle_buffer_try_push(buffer, (void*)4);
    void *j;
    circle_buffer_try_pop(buffer, &j);
    printf("j: %lli\n", j);
    circle_buffer_print(buffer);
}

// buffer full
void test1()
{
    printf("---Test 1---\n");
    void *buffer_ptr = circle_buffer_constructor(3);
    if (buffer_ptr == NULL)
    {
        printf("Failed to create buffer\n");
    }

    void *j;
    circle_buffer_print(buffer_ptr);

    circle_buffer_try_push(buffer_ptr, (void *)2);
    circle_buffer_try_push(buffer_ptr, (void *)3);
    circle_buffer_try_pop(buffer_ptr, &j);
    printf("j: %lli\n", j);
    circle_buffer_try_pop(buffer_ptr, &j);
    printf("j: %lli\n", j);
    assert(circle_buffer_try_pop(buffer_ptr, &j) == RET_EMPTY);

    circle_buffer_try_push(buffer_ptr, (void *)4);
    circle_buffer_try_push(buffer_ptr, (void *)5);
    
    assert( circle_buffer_try_push(buffer_ptr, (void *)6) == RET_FULL);
    circle_buffer_print(buffer_ptr);
   
}

// Normal use
void test2()
{
    printf("---Test 2---\n");
    void *buffer_ptr = circle_buffer_constructor(5);
    if (buffer_ptr == NULL)
    {
        printf("Failed to create buffer\n");
    }

    void *j;
    circle_buffer_print(buffer_ptr);

    for (int i = 0; i < 30; i++)
    {
        circle_buffer_try_push(buffer_ptr, (void *)i);
        circle_buffer_try_pop(buffer_ptr, &j);
        printf("j: %lli\n", j);
    }
    circle_buffer_print(buffer_ptr);
}

// pop empty
void test3()
{
    printf("---Test 3---");

    void *buffer_ptr = circle_buffer_constructor(5);
    if (buffer_ptr == NULL)
    {
        printf("Failed to create buffer\n");
    }

    void *j;
    circle_buffer_print(buffer_ptr);

    circle_buffer_try_push(buffer_ptr, (void *)1);
    circle_buffer_try_pop(buffer_ptr, &j);
    printf("j: %lli\n", j);
    circle_buffer_try_pop(buffer_ptr, &j);
    printf("j: %lli\n", j);

    circle_buffer_print(buffer_ptr);
}

int main()
{
    // test_stack();

    test_heap();

    test1();

    // test2();

    // test3();
}