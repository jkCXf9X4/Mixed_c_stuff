

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
#include <stdint.h>
#include <stdatomic.h>

#include "time_it.h"
#include "semaphore.h"

#define MESSAGE_SIZE 32

typedef struct message
{
    int a;
    int b;
    char c[MESSAGE_SIZE];
} message_t;

typedef struct shm
{
    _Atomic bool empty;
    message_t message;
} shm_t;

void *sem;
void *sem2;

pthread_mutex_t mutex, mutex2;
pthread_spinlock_t spinnlock, spinnlock2;

shm_t *shared_memory;
int no_of_messages = 0;
int sum = 0;

void *send(void (*func)(int i))
{
    void *time_it = time_it_constructor();
    time_it_set_start_time(time_it);
    for (int i = 0; i < no_of_messages; i++)
    {
        // printf("%s: i: %i\n", __func__, i);
        func(i);
    }
    time_it_set_end_time(time_it);
    // printf("%s: Function finished\n", __func__);
    time_it_set_counter(time_it, no_of_messages);
    time_it_print_results(time_it, stdout, NULL);
}

void *receive(void (*func)(void))
{
    int counter = 0;
    while (counter < no_of_messages)
    {
        // printf("%s: counter: %i\n", __func__, counter);
        func();
        counter++;
    }
    // printf("%s: Function finished\n", __func__);
}

void set_shm_atomic(int i)
{
    // printf("%s: Waiting to send\n", __func__);
    while (atomic_load(&shared_memory->empty) != true)
        ;
    // printf("%s: Chreating message i: %i\n", __func__, i);
    shared_memory->message.a = i;
    // printf("%s: Set empty to false\n", __func__);
    atomic_store(&shared_memory->empty, false);
    // printf("%s: Done\n", __func__);
}

void get_shm_atomic()
{
    // printf("%s: Waiting for receive\n", __func__);
    while (atomic_load(&shared_memory->empty) != false)
        ;
    // printf("%i\n", shared_memory->message.a);
    sum += shared_memory->message.a;
    // printf("%s: Seting empty to true\n", __func__);
    atomic_store(&shared_memory->empty, true);
}

void *send_atomic_pointer(void *ptr)
{
    send(set_shm_atomic);
}

void *receive_atomic_pointer(void *ptr)
{
    receive(get_shm_atomic);
}

void set_shm_semaphore(int i)
{
    sem_wait(sem2);
    shared_memory->message.a = i;
    sem_run_all(sem);
}

void get_shm_semaphore()
{
    sem_wait(sem);
    sum += shared_memory->message.a;
    sem_run_all(sem2);
}

void *send_semaphore(void *ptr)
{
    send(set_shm_semaphore);
}

void *receive_semaphore(void *ptr)
{
    sleep(1);
    sem_run_all(sem2);
    receive(get_shm_semaphore);
}

void set_shm_mutex(int i)
{
    pthread_mutex_lock(&mutex);
    shared_memory->message.a = i;
    pthread_mutex_unlock(&mutex2);
}

void get_shm_mutex()
{
    pthread_mutex_lock(&mutex2);
    // printf("Received %i\n", shared_memory->message.a);
    sum += shared_memory->message.a;
    pthread_mutex_unlock(&mutex);
}

void *send_mutex(void *ptr)
{
    send(set_shm_mutex);
}

void *receive_mutex(void *ptr)
{
    receive(get_shm_mutex);
}

void set_shm_spinlock(int i)
{
    pthread_spin_lock(&spinnlock);
    shared_memory->message.a = i;
    pthread_spin_unlock(&spinnlock2);
}

void get_shm_spinlock()
{
    pthread_spin_lock(&spinnlock2);
    // printf("Received %i\n", shared_memory->message.a);
    sum += shared_memory->message.a;
    pthread_spin_unlock(&spinnlock);
}

void *send_spinlock(void *ptr)
{
    send(set_shm_spinlock);
}

void *receive_spinlock(void *ptr)
{
    receive(get_shm_spinlock);
}

/* main.c */
int main(int argc, char *argv[])
{
    pthread_t thread1, thread2;

    no_of_messages = 100;

    int comp_sum = 0;
    for (int i = 0; i < no_of_messages; i++)
    {
        comp_sum += i;
    }
    printf("%s: Result should be %i\n", __func__, comp_sum);

    printf("---ATOMIC---\n");
    shared_memory = malloc(sizeof(shm_t));
    shared_memory->message.a = 0;
    atomic_init(&shared_memory->empty, true);

    pthread_create(&thread1, NULL, send_atomic_pointer, NULL);
    pthread_create(&thread2, NULL, receive_atomic_pointer, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    printf("Sum %i\n", sum);

    free(shared_memory);

    printf("---SEMEPHORE---\n");
    shared_memory = malloc(sizeof(shm_t));
    shared_memory->message.a = 0;
    sum = 0;
    sem = sem_constructor(NULL);
    sem2 = sem_constructor(NULL);

    pthread_create(&thread1, NULL, send_semaphore, NULL);
    pthread_create(&thread2, NULL, receive_semaphore, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    printf("Sum %i\n", sum);

    free(shared_memory);

    printf("---MUTEX---\n");
    sum = 0;
    shared_memory = malloc(sizeof(shm_t));
    shared_memory->message.a = 0;

    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&mutex2, NULL);

    pthread_mutex_lock(&mutex2);

    pthread_create(&thread1, NULL, send_mutex, NULL);
    pthread_create(&thread2, NULL, receive_mutex, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    printf("Sum %i\n", sum);
    free(shared_memory);

    printf("---SPINLOCK---\n");
    sum = 0;
    shared_memory = malloc(sizeof(shm_t));
    shared_memory->message.a = 0;

    pthread_spin_init(&spinnlock, PTHREAD_PROCESS_SHARED);
    pthread_spin_init(&spinnlock2, PTHREAD_PROCESS_SHARED);

    pthread_spin_lock(&spinnlock2);
    pthread_create(&thread1, NULL, send_spinlock, NULL);
    pthread_create(&thread2, NULL, receive_spinlock, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    printf("Sum %i\n", sum);
    free(shared_memory);

    exit(0);
}
