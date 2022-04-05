#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <libgen.h>
#include <errno.h>
#include <string.h>
#include <getopt.h>
#include <sys/types.h>
#include <pthread.h>

#include "semaphore.h"

void *sem_constructor(int *status)
{
    binary_semaphore_t *semaphore = malloc(sizeof(binary_semaphore_t));
    if (semaphore == NULL)
    {
        printf("failed to allocate semaphore");
        *status = -1;
    }
    int temp_status = sem_init(semaphore);
    if (status != NULL)
        *status = temp_status;
    return semaphore;
}

int sem_init(void *semaphore_obj)
{
    binary_semaphore_t *semaphore = semaphore_obj;
    pthread_mutex_init(&semaphore->mutex, NULL);
    pthread_cond_init(&semaphore->cond, NULL);
    semaphore->int_cond = 0;
    return 0;
}

void sem_wait(void *semaphore_obj)
{
    binary_semaphore_t *semaphore = semaphore_obj;
    pthread_mutex_lock(&semaphore->mutex);
    while (semaphore->int_cond != 1) //spurious awakening
    {
        pthread_cond_wait(&semaphore->cond, &semaphore->mutex);
    }
    semaphore->int_cond = 0;
    pthread_mutex_unlock(&semaphore->mutex);
}

void sem_run_all(void *semaphore_obj)
{
    binary_semaphore_t *semaphore = semaphore_obj;
    pthread_mutex_lock(&semaphore->mutex);
    semaphore->int_cond = 1;
    pthread_cond_broadcast(&semaphore->cond);
    pthread_mutex_unlock(&semaphore->mutex);
}

