#ifndef __DECODER_INCLUDED__
#define __DECODER_INCLUDED__

#include <stdint.h>
#include <stdio.h>
#include <limits.h>
#include "tree.h"
#include "huffman_algo.h"

#define BUFFER_SIZE 1024
#define BITS_IN_BYTE 8
#define BYTES_BUFFER_SIZE (BUFFER_SIZE/CHAR_BIT)
#define ASCII_LEN 128
#define BIT_BUFFER_SIZE (BUFFER_SIZE/CHAR_BIT)

void
expand(char buffer[BUFFER_SIZE], char bit_buffer[BIT_BUFFER_SIZE]) {
	int k = 0;
	for (int i=0; i<BIT_BUFFER_SIZE; i++) {
		for (int j=CHAR_BIT-1; j>=0; j--) {
			buffer[k++] = (bit_buffer[i] & (1<<j))>>j;
		}
	}
}

void
decode(const char *in_file_name, const char *out_file_name) {
	FILE *in_file = fopen(in_file_name, "rb");
	FILE *out_file = fopen(out_file_name, "w");

	int freqs[ASCII_LEN];
	fread(freqs, sizeof(int), ASCII_LEN, in_file);

	Node *root = form_huffman_tree(freqs);

	char bit_buffer[BUFFER_SIZE/BITS_IN_BYTE];
	char buffer[BUFFER_SIZE];
	int numchar;

	while (1) {
		fread(&numchar, sizeof(int), 1, in_file);
		if (numchar==0) {
			break;
		}

		fread(bit_buffer, sizeof(char), BIT_BUFFER_SIZE, in_file);
		expand(buffer, bit_buffer);

		int k = 0;
		for (int j=0; j<numchar; j++) {
			Node *current = root;
			while (!current->isLeaf) {
				if (buffer[k++]==0) {
					current = current->left;
				} else {
					current = current->right;
				}
			}

			fputc(current->ch, out_file);
			//printf("%c", current->ch);
		}
	}

	fclose(in_file);
	fclose(out_file);
}
#endif //__DECODER_INCLUDED__
