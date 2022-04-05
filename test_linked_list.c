

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <libgen.h>
#include <errno.h>
#include <string.h>
#include <getopt.h>
#include <sys/types.h>
#include <pthread.h>
#include <time.h>

#include "stack.h"
#include "linked_list.h"
#include "time_it.h"

#define SECOND_IN_NANOSECOND 1000000000
#define SECOND_IN_MILLISECOND 1000

/* main.c */
int main(int argc, char *argv[])
{
    printf("\nSTART\n");

    void *time_it = time_it_constructor();
    // void *linked_list = linked_list_constructor();


    // Test malloc/free
    int total_operations = 1000;
    size_t allocation_size = 1024;
    time_it_set_start_time(time_it);

    for (int i = 0; i < total_operations; i++)
    {
        void * data = malloc(allocation_size);
        free(data);
    }

    time_it_set_end_time(time_it);
    time_it_set_counter(time_it, total_operations);
    time_it_print_results(time_it, stdout, NULL);


    // Test free list

    total_operations = 1000;
    void *stack = stack_constructor(sizeof(void*), 50, 50);
    
    time_it_set_start_time(time_it);
    
    for (int i = 0; i < total_operations; i++)
    {
        void * data = stack_new(stack);
        stack_pop(stack);
    }
  
    time_it_set_end_time(time_it);
    
    stack_print(stack);

    
    time_it_set_counter(time_it, total_operations);
    time_it_print_results(time_it, stdout, NULL);   

    
    printf("\nEND\n");
}