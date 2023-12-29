#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

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
	memset(node, 0, sizeof(TreeNode));
	return node;
}

void tree_node_release(TreeNode *node)
{
	free(node);
}

#endif//__TREE_INCLUDED__
