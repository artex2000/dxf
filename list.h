#ifndef __LINKED_LIST__H__
#define __LINKED_LIST__H__


typedef struct _list_item list_item;

struct _list_item {
	list_item *next;
	list_item *prev;
	void *data;
};

void init_list (list_item *head);
list_item * get_first_node (list_item *head);
list_item * get_next_node (list_item *head, list_item *node);
list_item * get_last_node (list_item *head);
void insert_head (list_item *head, list_item *node);
void insert_tail (list_item *head, list_item *node);
void insert_before (list_item *mark, list_item *node);
void insert_after (list_item *mark, list_item *node);
void remove_node (list_item *node);
int is_list_empty (list_item *head);
int is_last_node (list_item *head, list_item *node);
int is_end_list (list_item *head, list_item *node);



#endif
