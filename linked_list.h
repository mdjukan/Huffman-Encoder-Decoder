#include "tree.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#ifndef __LIST_INCLUDED__
#define __LIST_INCLUDED__

struct ListNode {
	struct ListNode *next, *prev;
	TreeNode *node;
	int freq;
};

typedef struct ListNode ListNode;

typedef struct {
	ListNode *sentinel;
	int size;
} LinkedList;

ListNode *list_node_create(TreeNode *tree_node, int freq)
{
	ListNode *node = (ListNode*)malloc(sizeof(ListNode));
	assert(node!=NULL);
	node->next = NULL;
	node->prev = NULL;
	node->node = tree_node;
	node->freq = freq;

	return node;
}

LinkedList *linked_list_create(void)
{
	LinkedList *list = (LinkedList*)malloc(sizeof(LinkedList));
	assert(list!=0);

	list->sentinel = list_node_create(NULL, 0);
	list->sentinel->next = list->sentinel;
	list->sentinel->prev = list->sentinel;
	list->size = 0;

	return list;
}

void list_node_release(ListNode *node)
{
	free(node);
}

void linked_list_add_sorted(LinkedList *list, ListNode *node)
{
	assert(list->sentinel!=NULL);
	ListNode *current = list->sentinel->next;
	while (current!=list->sentinel && current->freq<=node->freq) {
		current = current->next;
	}

	ListNode *before_current = current->prev;
	before_current->next = node;
	node->prev = before_current;

	node->next = current;
	current->prev = node;

	list->size += 1;
}

ListNode *linked_list_pop_head(LinkedList *list) {
	assert(list->size != 0);

	ListNode *head = list->sentinel->next;
	ListNode *after_head = head->next;

	list->sentinel->next = after_head;
	after_head->prev = list->sentinel;

	head->next = NULL;
	head->prev = NULL;

	list->size -= 1;
	return head;
}

void linked_list_print(LinkedList *list)
{
	printf("{list} : ");
	if (list->size==0) {
		printf("[empty]");
	} else {
		ListNode *current = list->sentinel->next;
		while (current!=list->sentinel) {
			printf("[c: %c | f:%d] ", current->node->value, current->freq);
			current = current->next;
		}
	}

	printf("\n");
}

#endif// __LIST_INCLUDED__
