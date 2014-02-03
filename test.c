#include <stdio.h>
#include <stdlib.h>

#include "memchunk.h"                                                      	

static void print_chunk_list(struct memchunk *, int, int);

int main(int argc, char ** argv) {
	struct memchunk chunklist[9];
	int argSize = 9, c, size;
	
	while ((c = getopt(argc, argv, "n:")) != -1) {
		switch (c) {
		case 'n':
			argSize = strtol(optarg, NULL, 10);	
			break;
		default:
			break;
		}
	}

	size = get_mem_layout(chunklist, argSize);
	printf("Chunks Found: %d  Chunks Stored: %d\n", size,
	    (argSize > size) ? size : argSize);
	print_chunk_list(chunklist, argSize, size);
	return 0;
}

void print_chunk_list(struct memchunk *chunklist, int size,
    int foundSize) {
	int i, listSize;
	if (size >= foundSize) {
		listSize = foundSize;
	} else {
		listSize = size;
	}
	for (i=0; i < listSize; i++) {
		printf("Start: %-15p Size: %-15lu  RW: %d\n",
		    chunklist[i].start,
		    chunklist[i].length,
		    chunklist[i].RW);
	}
}
