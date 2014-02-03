#include <setjmp.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct 	memchunk {
	void *start;
	unsigned long length;
	int RW;
};

int 	get_mem_layout(struct memchunk *, int);
