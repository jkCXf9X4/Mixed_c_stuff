#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <libgen.h>
#include <errno.h>
#include <string.h>
#include <getopt.h>
#include <sys/types.h>
#include <pthread.h>

typedef struct binary_semaphore
{
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int int_cond;
} binary_semaphore_t;

void *sem_constructor(int *status);

int sem_init(void *semaphore);

void sem_wait(void *semaphore);

void sem_run_all(void *semaphore);

