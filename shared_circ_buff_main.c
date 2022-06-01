

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

#include "circle_buffer.h"

#define MESSAGE_POOL_LENGTH 20
#define MESSAGE_SIZE 32

typedef struct message
{
    int a;
    int b;
    char c[MESSAGE_SIZE];
} message_t;

void *cir_buffer;
int buffer_size = MESSAGE_POOL_LENGTH;
message_t message_pool[MESSAGE_POOL_LENGTH];

int received_no_of_messages = 0;
int sum = 0;

void set_circ_buffer(int i)
{
    struct timespec ts = {0, 1000};
    while (circle_buffer_is_full(cir_buffer))
    {
        nanosleep(&ts, NULL);
        ts.tv_nsec += 1000;
    }

    int index = circle_buffer_get_write_index(cir_buffer);
    message_t *send_message = &message_pool[index];
    send_message->b = i;

    circle_buffer_push(cir_buffer, send_message);
}

void get_circ_buff()
{
    struct timespec ts = {0, 1000};
    while (circle_buffer_is_empty(cir_buffer))
    {
        nanosleep(&ts, NULL);
        ts.tv_nsec += 1000;
    }

    message_t *j;
    circle_buffer_read_next(cir_buffer, (void **)&j);
    sum += j->b;

    circle_buffer_pop(cir_buffer, NULL);
}

void *send(void *ptr)
{
    void *time_it = time_it_constructor();
    time_it_set_start_time(time_it);
    for (int i = 0; i < received_no_of_messages; i++)
    {
        // printf("%s: i: %i\n", __func__, i);
        set_circ_buffer(i);
    }
    time_it_set_end_time(time_it);
    // printf("%s: Function finished\n", __func__);
    time_it_set_counter(time_it, received_no_of_messages);
    time_it_print_results(time_it, stdout, NULL);
}

void *receive(void *ptr)
{
    int counter = 0;
    while (counter < received_no_of_messages)
    {
        // printf("%s: counter: %i\n", __func__, counter);
        get_circ_buff();
        counter++;
    }
    // printf("%s: Function finished\n", __func__);
}


/* main.c */
int main(int argc, char *argv[])
{
    pthread_t thread1, thread2;

    received_no_of_messages = 1000000;

    int comp_sum = 0;
    for (int i = 0; i < received_no_of_messages; i++)
    {
        comp_sum += i;
    }
    printf("%s: Result should be %i\n", __func__, comp_sum);

    printf("---Circ buffer---\n");
    cir_buffer = circle_buffer_constructor(20);

    pthread_create(&thread1, NULL, send, NULL);
    pthread_create(&thread2, NULL, receive, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    printf("Sum %i\n", sum);
    printf(comp_sum == sum ? "Correct\n" : "Wrong answer\n");

    free(cir_buffer);

    exit(0);
}
