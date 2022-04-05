
#include <stdbool.h>

// Object used for measuring distance between times

#define SECOND_IN_MILLISECOND 1000
#define SECOND_IN_MICROSECOND 1000000
#define SECOND_IN_NANOSECOND 1000000000
#define NANOSECOND_IN_MILLISECOND 1000000

typedef struct time_it
{
    struct timespec start_time;
    struct timespec end_time;
    bool start_is_set;
    bool end_is_set;
    long operations;
    time_t result_time;
    time_t result_time_per_op;
} time_it_t;

// Allocates object
// void * - return pointer to memory segment

void *time_it_allocate();

// Initializes object, can also be used to reset object
// void *obj - time_it object
// int - RET_OK/RET_ERROR
int time_it_init(void *obj);

// Allocates and initializes object
// void * - return pointer to memory segment

void *time_it_constructor();

// Destroys the object and free the memory
// void *time_it_obj - time_it object
// bool free_obj - true is obj is allocated on heap, false if stack
// int - RET_OK/RET_ERROR
int time_it_destructor(void *time_it_obj, bool free_obj);

// Set start measuring time
// void *obj - time_it object
// int - RET_OK/RET_ERROR
int time_it_set_start_time(void *obj);

// Set end measuring time
// void *obj - time_it object
// int - RET_OK/RET_ERROR
int time_it_set_end_time(void *obj);

// Increase counter by 1
// Usage: insert in loop if number of operations is unknown, will cause a small increase in the measurement
// void *obj - time_it object
// int - RET_OK/RET_ERROR
int time_it_increase_counter(void *obj);

// Set the counter to a specific value
// void *obj - time_it object
// int - RET_OK/RET_ERROR
int time_it_set_counter(void *obj, long operations);

// Calculate and get the resulting total time
// void *obj - time_it object
// long long out_resulting_time - [out] Total time in nano seconds
// int - RET_OK/RET_ERROR
int time_it_get_result_time(void *obj, time_t *out_result_time);

// Get the resulting time per operation
// void *obj - time_it object
// double out_result_time_per_op - [out] Time per operation in nano seconds
// int - RET_OK/RET_ERROR
int time_it_get_result_time_per_op(void *obj, time_t *out_result_time_per_op);

// Print the result
// void *obj - time_it object
// FILE * stream - stdout, stderr or file
// char * opt_description - [opt][NULL] Additional description in printout
// int - RET_OK/RET_ERROR
int time_it_print_results(void *obj, FILE *stream, char *opt_description);