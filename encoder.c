#include <stdio.h>
#include <string.h>
#include "linked_list.h"
#include <ctype.h>

#define ASCII_SIZE 128
#define INPUT_FILE "huff.txt"

void count_freqs(char *file_name, int freqs[ASCII_SIZE])
{
	FILE *f = fopen(file_name, "r");
	assert(f!=NULL);

	for (int i=0; i<ASCII_SIZE; ++i) {
		freqs[i] = 0;
	}

	char c;
	while ((c=fgetc(f))!=EOF) {
		freqs[(int)c] += 1;
	}

	fclose(f);
}

LinkedList *create_list(int freqs[ASCII_SIZE])
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

#define CODE_BUFF_SIZE 30
char codes[ASCII_SIZE][CODE_BUFF_SIZE];

void add_code(char value, char code[])
{
	for (int i=0; i<CODE_BUFF_SIZE; ++i) {
		codes[(int)value][i] = code[i];
	}
}

void write_codes(TreeNode *root, char code[], int end)
{
	if (root->left==NULL && root->right==NULL) {
		code[end] = '\0';
		add_code(root->value, code);
	} else {
		if (root->left!=NULL) {
			code[end++] = '0';
			write_codes(root->left, code, end);
			end --;
		}

		if (root->right!=NULL) {
			code[end++] = '1';
			write_codes(root->right, code, end);
			end --;
		}
	}
}

int main()
{
	int freqs[ASCII_SIZE];
	count_freqs(INPUT_FILE, freqs);
	LinkedList *list = create_list(freqs);

	TreeNode *root = form_huffman_tree(list);
	char code_buffer[CODE_BUFF_SIZE];
	write_codes(root, code_buffer, 0);

	int bits_needed = 0;
	int symb_count = 0 ;
	for (int i=0; i<ASCII_SIZE; ++i) {
		if(freqs[i]!=0) {
			if (isprint(i)) {
				printf("[%c] - %3d - ", i, freqs[i]);
			} else {
				printf("[\\] - %3d - ", freqs[i]);
			}

			printf("%s\n", codes[i]);
			bits_needed += strlen(codes[i]) * freqs[i];
			symb_count += freqs[i];
		}
	}

	printf("bits_needed -> %d\n", bits_needed);
	printf("symb_count -> %d\n", symb_count);
}
