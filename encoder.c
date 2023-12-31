#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "linked_list.h"
#include "tree.h"
#include "huffman_algo.c"
#include "tree_data.c"

#define INPUT_FILE "huff"

void add_codes(TreeNode *root, char **codes, char *buffer, int *buff_end)
{
	if (root->left==NULL && root->right==NULL) {
		buffer[*buff_end] = '\0';
		for (int i=0; i<=*buff_end; ++i) {
			codes[(int)(root->value)][i] = buffer[i];
		}
	} else {
		buffer[*buff_end] = '0';
		*buff_end += 1;
		add_codes(root->left, codes, buffer, buff_end);
		*buff_end -= 1;

		buffer[*buff_end] = '1';
		*buff_end += 1;
		add_codes(root->right, codes, buffer, buff_end);
		*buff_end -= 1;
	}
}

char **codes_from_tree(TreeNode *root)
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
	add_codes(root, codes, buffer, &buff_end);

	return codes;
}

int main()
{
	TreeNode *root = huffman_tree(INPUT_FILE);
	TreeNode_array *node_array = tree_to_node_array(root);
	char **tree_codes = codes_from_tree(root);
	char **array_codes = codes_from_array(node_array);
	int all_good = 0;
	for (int i=0; i<ASCII_SIZE; ++i) {
		int code_diff = strcmp(tree_codes[i], array_codes[i]);
		all_good += code_diff;
	}

	printf("all good -> %d\n", all_good);
}
