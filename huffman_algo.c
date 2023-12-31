#include "linked_list.h"
#include "tree.h"
#include "consts.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#ifndef __HUFFMAN_ALGO_INCLUDED__
#define __HUFFMAN_ALGO_INCLUDED__

int *count_freqs(char *file_name )
{
	int *freqs = (int*)malloc(ASCII_SIZE * sizeof(int));
	assert(freqs!=NULL);
	for (int i=0; i<ASCII_SIZE; ++i) {
		freqs[i] = 0;
	}

	int fd = open(file_name, O_RDONLY);
	char c;
	while (read(fd, (void*)&c, 1)>0) {
		freqs[(int)c] += 1;
	}
	close(fd);

	freqs['\n'] -= 1;
	return freqs;
}

LinkedList *create_list(int *freqs)
{
	LinkedList *list = linked_list_create();

	for (int i=0; i<ASCII_SIZE; ++i) {
		if (freqs[i]!=0) {
			TreeNode *tree_node = tree_node_create();
			tree_node->value = (char)i;
			ListNode *list_node = list_node_create(tree_node, freqs[i]);
			linked_list_add_sorted(list, list_node);
		}
	}

	return list;
}

TreeNode *form_huffman_tree(LinkedList *list)
{
	while (list->size!=1) {
		ListNode *node1 = linked_list_pop_head(list);
		ListNode *node2 = linked_list_pop_head(list);

		TreeNode *new_tree_node = tree_node_create();
		new_tree_node->left = node1->node;
		new_tree_node->right = node2->node;
		new_tree_node->value = 0;

		int new_freq = node1->freq + node2->freq;
		ListNode *new_list_node = list_node_create(new_tree_node, new_freq);
		linked_list_add_sorted(list, new_list_node);

		list_node_release(node1);
		list_node_release(node2);
	}

	ListNode *last = linked_list_pop_head(list);
	TreeNode *root = last->node;
	list_node_release(last);
	return root;
}

TreeNode *huffman_tree(char *file_name)
{
	int *freqs = count_freqs(file_name);
	LinkedList *node_list = create_list(freqs);
	TreeNode *root = form_huffman_tree(node_list);
	linked_list_release(node_list);
	free(freqs);

	return root;
}

#endif//__HUFFMAN_ALGO_INCLUDED__
