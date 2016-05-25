#include <stdlib.h>

#include "list.h"


void init_list (list_item *head)
{
	head->next = head;
	head->prev = head;
	head->data = NULL;
}

list_item * get_first_node (list_item *head)
{
	return head->next;
}

list_item * get_last_node (list_item *head)
{
	return head->prev;
}

list_item * get_next_node (list_item *head, list_item *node)
{
	list_item *p = head;
	while (p != node)
		p = p->next;
	return p->next;
}

void insert_head (list_item *head, list_item *node)
{
	node->next = head->next;
	node->prev = head;
	node->next->prev = node;
	head->next = node;
}

void insert_tail (list_item *head, list_item *node)
{
	node->next = head;
	node->prev = head->prev;
	node->prev->next = node;
	head->prev = node;
}

void insert_before (list_item *mark, list_item *node)
{
	insert_tail (mark, node);
}

void insert_after (list_item *mark, list_item *node)
{
	insert_head (mark, node);
}

void remove_node (list_item *node)
{
	node->prev->next = node->next;
	node->next->prev = node->prev;
}

int is_list_empty (list_item *head)
{
	return (head->next == head) ? 1 : 0;
}

int is_last_node (list_item *head, list_item *node)
{
	return (node->next == head) ? 1 : 0;
}

int is_end_list (list_item *head, list_item *node)
{
	return (node == head) ? 1 : 0;
}
