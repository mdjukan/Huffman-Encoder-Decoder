#include <assert.h>
#include <stdlib.h>
#include "tree.h"
#include "consts.h"

#ifndef __TREE_DATA_INCLUDED__
#define __TREE_DATA_INCLUDED__

typedef struct {
	int left_index, right_index;
	char value;
} TreeNode_array;

int add_node_to_array(TreeNode *root, TreeNode_array *tree_array, int *first_free)
{
	if (root==NULL) {
		return -1;
	}

	int node_index = *first_free;
	*first_free += 1;
	int left_index = add_node_to_array(root->left, tree_array, first_free);
	int right_index = add_node_to_array(root->right, tree_array, first_free);
	TreeNode_array node = {left_index, right_index, root->value};
	tree_array[node_index] = node;

	return node_index;
}

TreeNode_array *tree_to_node_array(TreeNode *root)
{
	assert(root!=NULL);

	int tree_size = tree_node_count(root);
	TreeNode_array *tree_array = malloc(tree_size * sizeof(TreeNode_array));
	assert(tree_array!=NULL);

	int first_empty = 0;
	int r = add_node_to_array(root, tree_array, &first_empty);
	assert(r==0);

	return tree_array;
}

void print_tree_array(TreeNode_array *tree_array, int tree_size)
{
	for (int i=0; i<tree_size; ++i) {
		printf("[%d] l->%d r->%d char->%c\n", i, tree_array[i].left_index, tree_array[i].right_index, tree_array[i].value);
	}
}

void add_codes_array(TreeNode_array *node_array, int node_index, char **codes, char *buffer, int *buff_end)
{
	TreeNode_array node = node_array[node_index];
	if (node.left_index==-1 && node.right_index==-1) {
		buffer[*buff_end] = '\0';
		for (int i=0; i<=*buff_end; ++i) {
			codes[(int)node.value][i] = buffer[i];
		}
	} else {
		buffer[*buff_end] = '0';
		*buff_end += 1;
		add_codes_array(node_array, node.left_index, codes, buffer, buff_end);
		*buff_end -= 1;

		buffer[*buff_end] = '1';
		*buff_end += 1;
		add_codes_array(node_array, node.right_index, codes, buffer, buff_end);
		*buff_end -= 1;
	}
}

char **codes_from_array(TreeNode_array *node_array)
{
	char **codes = (char**)malloc(ASCII_SIZE * sizeof(char*));
	assert(codes!=NULL);
	for (int i=0; i<ASCII_SIZE; ++i) {
		codes[i] = (char*)malloc(CODE_BUFFER_SIZE * sizeof(char));
		assert(codes[i]!=NULL);
		memset(codes[i], 0, CODE_BUFFER_SIZE * sizeof(char));
	}

	char *buffer = (char*)malloc(CODE_BUFFER_SIZE * sizeof(char));
	assert(buffer!=NULL);

	int buff_end = 0;
	add_codes_array(node_array, 0, codes, buffer, &buff_end);

	return codes;
}

#endif//__TREE_DATA_INCLUDED__
