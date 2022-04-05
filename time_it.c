

#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include <stdlib.h>

#include "time_it.h"

// Object used for measuring distance between times

// Allocates time_it object
// void * - return pointer to memory segment

void *time_it_allocate()
{
    return malloc(sizeof(time_it_t));
}

// Initializes object, can also be used to reset object
// void *time_it_obj - time_it object
// int - RET_OK/RET_ERROR
int time_it_init(void *time_it_obj)
{
    time_it_t *self = time_it_obj;
    self->operations = 0;
    self->start_is_set = false;
    self->end_is_set = false;
    return 0;
}

// Allocates and initializes object
// void * - return pointer to memory segment

void *time_it_constructor()
{
    time_it_t *time_it_obj = time_it_allocate();
    if (time_it_obj == NULL)
        return NULL;
    if (time_it_init(time_it_obj) != 0)
        return NULL;
    return time_it_obj;
}

// Destroys the object and free the memory
// void *time_it_obj - time_it object
// bool free_obj - true is obj is allocated on heap, false if stack
// int - RET_OK/RET_ERROR
int time_it_destructor(void *time_it_obj, bool free_obj)
{
    if (free_obj)
    {
        free(time_it_obj);
        time_it_obj = NULL;
    }
    return 0;
}

// Set start measuring time
// void *time_it_obj - time_it object
// int - RET_OK/RET_ERROR
int time_it_set_start_time(void *time_it_obj)
{
    time_it_t *self = time_it_obj;
    self->start_is_set = true;
    clock_gettime(CLOCK_MONOTONIC, &self->start_time);
    return 0;
}

// Set end measuring time
// void *time_it_obj - time_it object
// int - RET_OK/RET_ERROR
int time_it_set_end_time(void *time_it_obj)
{
    time_it_t *self = time_it_obj;
    clock_gettime(CLOCK_MONOTONIC, &self->end_time);
    self->end_is_set = true;
    return 0;
}

// Increase counter by 1
// Usage: insert in loop if number of operations is unknown, will cause a small increase in the measurement
// void *time_it_obj - time_it object
// int - RET_OK/RET_ERROR
int time_it_increase_counter(void *time_it_obj)
{
    time_it_t *self = time_it_obj;
    self->operations++;
    return 0;
}

// Set the counter to a specific value
// void *time_it_obj - time_it object
// int - RET_OK/RET_ERROR
int time_it_set_counter(void *time_it_obj, long operations)
{
    time_it_t *self = time_it_obj;
    self->operations = operations;
    return 0;
}

// Calculate and get the resulting total time
// void *time_it_obj - time_it object
// long long out_resulting_time - [out] Total time in nano seconds
// int - RET_OK/RET_ERROR
int time_it_get_result_time(void *time_it_obj, time_t *out_result_time)
{
    time_it_t *self = time_it_obj;
    if (!self->start_is_set || !self->end_is_set)
    {
        // NOTICE("Set start and end points prior to use\n")
        return -1;
    }

    time_t seconds = self->end_time.tv_sec - self->start_time.tv_sec;
    long nanos = self->end_time.tv_nsec - self->start_time.tv_nsec;
    time_t result_time = seconds * SECOND_IN_NANOSECOND + nanos;
    self->result_time = result_time; // save result in object
    *out_result_time = result_time;
    return 0;
}

// Get the resulting time per operation
// void *time_it_obj - time_it object
// double out_result_time_per_op - [out] Time per operation in nano seconds
// int - RET_OK/RET_ERROR
int time_it_get_result_time_per_op(void *time_it_obj, time_t *out_result_time_per_op)
{
    time_it_t *self = time_it_obj;
    time_t result_time;
    if (time_it_get_result_time(self, &result_time) != 0)
        return -1;
    if (self->operations == 0)
        self->operations = 1;
    time_t result_time_per_op = result_time / self->operations;
    self->result_time_per_op = result_time_per_op; // save result in object
    *out_result_time_per_op = result_time_per_op;
    return 0;
}

// Print the result
// void *time_it_obj - time_it object
// FILE * stream - stdout, stderr or file
// char * opt_description - [opt][NULL] Additional description in printout
// int - RET_OK/RET_ERROR
int time_it_print_results(void *time_it_obj, FILE *stream, char *opt_description)
{
    time_it_t *self = time_it_obj;
    time_t temp;
    time_it_get_result_time_per_op(self, &temp); // updates the result variabels, result_time and result_time_per_op

    if (stream == NULL)
    {
        // NOTICE("An output stream must be provided")
        return -1;
    }

    fprintf(stream, "%s: %s Operations: %i Time/op: %lli nanos, Time: %lli nanos\n",
            __func__, opt_description == NULL ? "" : opt_description, self->operations, self->result_time_per_op, self->result_time);
    return 0;
}