#include "list.h"

void list_push_head(struct list *list, struct list_node *node)
{
	node->next = list->head;
	node->prev = 0;
	if(list->head)
		list->head->prev = node;
	list->head = node;
	if(!list->tail)
		list->tail = node;
	node->list = list;
	list->size++;
}

void list_push_tail(struct list *list, struct list_node *node)
{
	node->prev = list->tail;
	node->next = 0;
	if(list->tail)
		list->tail->next = node;
	list->tail = node;
	if(!list->head)
		list->head = node;
	node->list = list;
	list->size++;
}

struct list_node *list_pop_head(struct list *list)
{
	struct list_node *result = list->head;
	if(list->head) {
		list->head = list->head->next;
		if(list->head)
			list->head->prev = 0;
		if(!list->head)
			list->tail = 0;
		result->next = result->prev = 0;
		result->list = 0;
		list->size--;
	}
	return result;
}

struct list_node *list_pop_tail(struct list *list)
{
	struct list_node *result = list->tail;
	if(list->tail) {
		list->tail = list->tail->prev;
		if(list->tail)
			list->tail->next = 0;
		if(!list->tail)
			list->head = 0;
		result->next = result->prev = 0;
		result->list = 0;
		list->size--;
	}
	return result;
}

void list_remove(struct list_node *node)
{
	if(!node->list)
		return;
	if(node->list->head == node) {
		list_pop_head(node->list);
		return;
	}
	if(node->list->tail == node) {
		list_pop_tail(node->list);
		return;
	}
	node->next->prev = node->prev;
	node->prev->next = node->next;
	node->next = node->prev = 0;
	node->list = 0;
	node->list->size--;
}

int list_size( struct list *list )
{
	return list->size;
}
