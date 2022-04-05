#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <libgen.h>
#include <errno.h>
#include <string.h>
#include <getopt.h>
#include <sys/types.h>
#include <pthread.h>

typedef struct block
{
    void * start;
    size_t items;
}block_t;

// allocator 

typedef struct block_allocator
{
    size_t blocks;
    size_t len_block_items;
    size_t item_size;
}block_allocator_t;

int block_allocator_init(block_allocator_t *allocator, size_t starting_blocks, size_t len_block_items, size_t item_size)
{

}

void *block_allocator_allocate(block_allocator *allocator)
{

}

int block_allocator_free(block_allocator *allocator, void* item)
{

}