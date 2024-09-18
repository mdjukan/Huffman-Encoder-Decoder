#ifndef __HUFFMAN_ALGO_INCLUDED__
#define __HUFFMAN_ALGO_INCLUDED__

#include "tree.h"
#include <stdlib.h>
#include <stdio.h>
#define ASCII_LEN 128

void
count_freqs(char *file_name, int freqs[ASCII_LEN]) {
	for (int i=0; i<ASCII_LEN; ++i) {
		freqs[i] = 0;
	}

	FILE *file = fopen(file_name, "r");
	if (file==NULL) {
		fprintf(stderr, "[ERROR] couldn't open file %s\n", file_name);
		exit(EXIT_FAILURE);
	}

	char ch;
	while ((ch = fgetc(file))!=EOF) {
		freqs[(int)ch] += 1;
	}

	//freqs[(int)'\n'] = 0;
	fclose(file);
}

void
add_node_sorted(Node *nodes[ASCII_LEN], Node *node) {
	int i=0;
	while (nodes[i]!=NULL && nodes[i]->freq<node->freq) {
		i ++;
	}

	for (int j=ASCII_LEN-2; j>=i; j--) {
		nodes[j+1] = nodes[j];
	}
	nodes[i] = node;
}

int
create_nodes(int freqs[ASCII_LEN], Node *nodes[ASCII_LEN]) {
	for (int i=0; i<ASCII_LEN; i++) {
		nodes[i] = NULL;
	}

	int num_nodes = 0;
	for (int i=0; i<ASCII_LEN; i++) {
		if (freqs[i]==0) {
			continue;
		}

		Node *node = create_node();
		node->isLeaf = true;
		node->freq = freqs[i];
		node->ch = (char)i;

		add_node_sorted(nodes, node);
		num_nodes += 1;
	}

	return num_nodes;
}

void
shift_2_left(Node *nodes[ASCII_LEN]) {
	for (int i=2; i<ASCII_LEN; i++) {
		nodes[i-2] = nodes[i];
	}
}

Node*
form_huffman_tree(int freqs[ASCII_LEN]) {
	Node *nodes[ASCII_LEN];
	int num_nodes = create_nodes(freqs, nodes);

	while (num_nodes!=1) {
		Node *left_node = nodes[0];
		Node *right_node = nodes[1];
		shift_2_left(nodes);

		Node *new_node = create_node();
		new_node->left = left_node;
		new_node->right = right_node;
		new_node->freq = left_node->freq + right_node->freq;
		new_node->isLeaf = false;

		add_node_sorted(nodes, new_node);
		num_nodes -= 1;
	}

	return nodes[0];
}

#define CODE_LEN (ASCII_LEN/2)

struct Code {
	char ch;
	int len;
	char code[CODE_LEN];
};

typedef struct Code Code;

Code*
create_code(char ch, char code[CODE_LEN], int len) {
	Code *c = (Code*)malloc(sizeof(Code));
	c->ch = ch;
	c->len = len;
	for (int i=0; i<len; i++) {
		c->code[i] = code[i];
	}

	return c;
}

void
write_codes(Node *root, Code *codes[ASCII_LEN], char code[CODE_LEN], int len) {
	if (root->isLeaf) {
		codes[(int)(root->ch)] = create_code(root->ch, code, len);
	} else {
		code[len] = 0;
		write_codes(root->left, codes, code, len+1);

		code[len] = 1;
		write_codes(root->right, codes, code, len+1);
	}
}

void
generate_codes(int freqs[ASCII_LEN], Code *codes[ASCII_LEN]) {
	Node *tree = form_huffman_tree(freqs);
	for (int i=0; i<ASCII_LEN; i++) {
		codes[i] = NULL;
	}

	char code[CODE_LEN];
	write_codes(tree, codes, code, 0);
	destroy_tree(tree);
}

/*
int main() {
	int freqs[ASCII_LEN];
	char input_file[] = "t8.txt";
	count_freqs(input_file, freqs);

	Code *codes[ASCII_LEN];
	generate_codes(freqs, codes);

	int bits_with = 0;
	int bits_without = 0;
	for (int i=0; i<ASCII_LEN; i++) {
		bits_without += 8 * freqs[i];
		if (freqs[i]!=0) {
			bits_with += freqs[i] * codes[i]->len;
		}
	}
	
	printf("Bits needed with Huffman's: %d\n", bits_with);
	printf("Bits needed without Huffman's: %d\n", bits_without);
	printf("Compression rate: %.2f\n", 100*(double)bits_with/bits_without);
}
*/

#endif//__HUFFMAN_ALGO_INCLUDED__
