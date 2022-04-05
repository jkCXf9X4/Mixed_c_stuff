
#define RET_OK 0
#define RET_ERROR -1

typedef struct node
{
    void *data;
    struct node *next;
    struct node *previous; 
}node_t;

typedef struct linked_list
{
    node_t *first_node;
    node_t *last_node;
    int count;
    int max;
}linked_list_t;

int linked_list_init(void *obj, int opt_max);

void *linked_list_constructor(int opt_max);

int linked_list_destructor(void *obj);

int linked_list_push_back(void *obj, void *item);

int linked_list_pop_back(void *obj, void **opt_out_item);

int linked_list_for_each_node(void *obj, void(*op)(void*));

int linked_list_for_each_item(void *obj, void(*op)(void*));

int linked_list_debug(void *obj);