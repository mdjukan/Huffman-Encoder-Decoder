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

struct Code {
	char ch;
	int len;
	char code;
};

typedef struct Code Code;

Code*
create_code(char ch, int len, char code) {
	Code *c = (Code*)malloc(sizeof(Code));
	c->ch = ch;
	c->len = len;
	c->code = code;
	return c;
}

#define BIT_0_MASK (((char)1))
#define BIT_1_MASK (((char)1)<<1)
#define BIT_2_MASK (((char)1)<<2)
#define BIT_3_MASK (((char)1)<<3)
#define BIT_4_MASK (((char)1)<<4)
#define BIT_5_MASK (((char)1)<<5)
#define BIT_6_MASK (((char)1)<<6)

char BIT_MASKS[] = {BIT_0_MASK,
	                   BIT_1_MASK,
	                   BIT_2_MASK,
	                   BIT_3_MASK,
	                   BIT_4_MASK,
	                   BIT_5_MASK,
	                   BIT_6_MASK,
                   };

void
add_code(Code **codes, Code *new_code) {
	int i=0;
	while (codes[i]!=NULL) {
		i ++;
	}

	codes[i] = new_code;
}

void
write_codes(Node *root, Code **codes, char code, int len) {
	if (root->isLeaf) {
		add_code(codes, create_code(root->ch, len, code));
	} else {
		char right_code = code | BIT_MASKS[len];
		char left_code = code;

		write_codes(root->left, codes, left_code, len+1);
		write_codes(root->right, codes, right_code, len+1);
	}
}

Code**
generete_codes(int freqs[ASCII_LEN]) {
	Node *tree = form_huffman_tree(freqs);
	Code **codes = (Code**)malloc(ASCII_LEN*sizeof(Code));
	for (int i=0; i<ASCII_LEN; i++) {
		codes[i] = NULL;
	}

	write_codes(tree, codes, 0, 0);
	destroy_tree(tree);
	return codes;
}

int get_code_len(Code **codes, char ch) {
	int i=0;
	while (codes[i]->ch != ch) {
		i ++;
	}

	return codes[i]->len;
}

int main() {
	int freqs[ASCII_LEN];
	char input_file[] = "t8.txt";
	count_freqs(input_file, freqs);

	Code **codes = generete_codes(freqs);
	/*
	int i=0;
	while (codes[i]!=NULL) {
		printf("%c - %d\n", codes[i]->ch, codes[i]->len);
		i ++;
	}
	*/

	int bits_with = 0;
	int bits_without = 0;
	for (int i=0; i<ASCII_LEN; i++) {
		bits_without += 8 * freqs[i];
		if (freqs[i]!=0) {
			bits_with += freqs[i] * get_code_len(codes, (char)i);
		}
	}
	
	printf("Bits needed with Huffman's: %d\n", bits_with);
	printf("Bits needed without Huffman's: %d\n", bits_without);
	printf("Compression rate: %.2f\n", 100*(double)bits_with/bits_without);
}

#endif//__HUFFMAN_ALGO_INCLUDED__
