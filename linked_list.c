
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#include "linked_list.h"

void *linked_list_allocate()
{
    return malloc(sizeof(linked_list_t));
}

int linked_list_init(void *linked_list_obj, int opt_max)
{
    linked_list_t *self = linked_list_obj;
    self->first_node = NULL;
    self->last_node = NULL;
    self->count = 0;
    if (opt_max == 0)
    {
        self->max = INT_MAX;
    }
    else
    {
        self->max = opt_max;
    }
}

void *linked_list_constructor(int opt_max)
{
    linked_list_t *object = linked_list_allocate();
    if (object == NULL) // Check allocated memory
    {
        return NULL;
    }
    linked_list_init(object, opt_max);
    return object;
}

int linked_list_destructor(void *linked_list_obj)
{

    // destroy stuff
    return 0;
}

int linked_list_push_back(void *linked_list_obj, void *item)
{
    linked_list_t *self = linked_list_obj;
    if (self->count >= self->max)
    {
        // NOTICE("Linked list maximum number of items is reached");
        return RET_ERROR;
    }

    node_t *new_node = malloc(sizeof(node_t));
    if (new_node == NULL) // Check allocated memory
    {
        return RET_ERROR;
    }
    new_node->data = item;
    new_node->next = NULL;
    new_node->previous = NULL;

    if (self->first_node == NULL) // fist item
    {
        self->first_node = new_node;
        self->last_node = new_node;
    }
    else
    {
        node_t *old_last = self->last_node;
        old_last->next = new_node;
        new_node->previous = old_last;
        self->last_node = new_node;
    }
    self->count++;
    return RET_OK;
}

int linked_list_pop_back(void *linked_list_obj, void **opt_out_item)
{
    linked_list_t *self = linked_list_obj;
    node_t *node_to_remove;

    if (self->count == 0)
    {
        return RET_OK;
    }
    if (self->count == 1)
    {
        node_to_remove = self->last_node;
        self->last_node = NULL;
        self->first_node = NULL;
    }

    node_to_remove = self->last_node;
    node_t *new_last = node_to_remove->previous;
    new_last->next = NULL;
    self->last_node = new_last;
    if (opt_out_item)
    {
        *opt_out_item = node_to_remove->data;
    }
    free(node_to_remove);
    self->count--;

    return RET_OK;
}

int linked_list_for_each_node(void *linked_list_obj, void (*op)(void *))
{
    linked_list_t *self = linked_list_obj;
    node_t *current = self->first_node;
    while (current != NULL)
    {
        op(current);
        current = current->next;
    }
    return RET_OK;
}

int linked_list_for_each_item(void *linked_list_obj, void (*op)(void *))
{
    linked_list_t *self = linked_list_obj;
    void op_item(void *node_)
    {
        node_t *node = node_;
        op(node->data);
    }
    linked_list_for_each_node(self, op_item);
}

int linked_list_debug(void *linked_list_obj)
{
    linked_list_t *self = linked_list_obj;
    int counter = 0;
    void op(void *node_)
    {
        node_t *node = node_;
        printf("%s: Counter: %i, Prev: %p, Node %p, Next %p, Data %p\n", __func__, counter, node->previous, node, node->next, node->data);
        counter++;
    }
    linked_list_for_each_node(self, op);
    return RET_OK;
}