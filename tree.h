#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "consts.h"

#ifndef __TREE_INCLUDED__
#define __TREE_INCLUDED__

struct TreeNode {
	struct TreeNode *left, *right;
	char value;
};

typedef struct TreeNode TreeNode;

TreeNode *tree_node_create(void)
{
	TreeNode *node = (TreeNode*)malloc(sizeof(TreeNode));
	assert(node!=NULL);

	node->left = NULL;
	node->right = NULL;
	node->value = 0;

	return node;
}

void tree_node_release(TreeNode *node)
{
	free(node);
}

void tree_release(TreeNode *root) {
	if (root!=NULL) {
		tree_release(root->left);
		tree_release(root->right);
		tree_node_release(root);
	}
}

int tree_node_count(TreeNode *root)
{
	if (root==NULL) {
		return 0;
	}

	return 1 + tree_node_count(root->left) + tree_node_count(root->right);
}

#endif//__TREE_INCLUDED__
