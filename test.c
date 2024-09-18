#include "encoder.h"
#include "decoder.h"
#include <stdbool.h>

bool equal(const char *file_name1, const char *file_name2) {
	FILE *file1 = fopen(file_name1, "r");
	FILE *file2 = fopen(file_name2, "r");

	while (1) {
		char c1 = fgetc(file1);
		char c2 = fgetc(file2);

		if (c1!=c2) {
			return false;
		}

		if (c1==c2 && c1==EOF) {
			return true;
		}
	}
}


int main() {
	encode("t8.txt", "t8.bin");
	decode("t8.bin", "t8_decoded.txt");
	if (equal("t8.txt", "t8_decoded.txt")) {
		printf("Original and decoded files are same!\n");
	} else {
		printf("Original and decode file differ!\n");
	}
}
