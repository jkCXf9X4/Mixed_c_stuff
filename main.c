

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
#include <limits.h>

// #include "linked_list.h"

#define SECOND_IN_NANOSECOND 1000000000
#define SECOND_IN_MILLISECOND 1000

typedef struct test
{
    int i;
} test_t;

/* main.c */
int main(int argc, char *argv[])
{ 

    // linked_list_debug(linked_list);
    printf("\nSTART\n");

    struct timespec start_time;
    struct timespec end_time;

#define MAX 100000
    test_t tests[MAX];

    int operations = 100000;

    clock_gettime(CLOCK_MONOTONIC, &start_time);

    for (int i = 0; i < operations; i++)
    {
        test_t *test = malloc(sizeof(test_t));
        tests[i] = *test;
        free(test);
    }

    clock_gettime(CLOCK_MONOTONIC, &end_time);

    int seconds = end_time.tv_sec - start_time.tv_sec;
    int nanos = end_time.tv_nsec - start_time.tv_nsec;
    printf("Sec %i, n_sec %i\n", seconds, nanos);

    long time = seconds * SECOND_IN_NANOSECOND + nanos;

    double time_per_operation = time / operations;
    printf("Time/op: %f nano\n", time_per_operation);
    printf("Time/op: %f milli\n", time_per_operation / SECOND_IN_MILLISECOND);
    printf("\nEND\n");

    // malloc & free 1024 -> ~1000 nanos
}