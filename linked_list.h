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

void list_node_release(ListNode *node)
{
	free(node);
}

void list_add_sorted(ListNode **head, ListNode *node)
{
	if (*head==NULL) {
		*head = node;
	} else {
		ListNode *before_current = NULL;
		ListNode *current = *head;
		while (current!=NULL && current->freq<=node->freq) {
			before_current = current;
			current = current->next;
		}

		if (current==NULL) {
			before_current->next = node;
			node->prev = before_current;
		} else	if (current==*head) { //before_current == NULL
			node->next = *head;
			(*head)->prev = node;
		} else {
			before_current->next = node;
			node->prev = before_current;
			current->prev = node;
			node->next = current;
		}
	}
}

ListNode *list_pop_head(ListNode **head) {
	assert(*head!=NULL);

	ListNode *old_head = *head;
	*head = old_head->next;

	if (*head!=NULL) {
		(*head)->prev = NULL;
	}

	old_head->next = NULL;
	return old_head;
}

#endif// __LIST_INCLUDED__
