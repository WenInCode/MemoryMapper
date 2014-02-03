#include "memchunk.h"

static void 	 my_handler(int);
static int 	 read_test(int *);
static int 	 write_test(int *, int);
static void	 set_chunk_list(struct memchunk *, void *, unsigned long, 
    int, int, int);

static sigjmp_buf env;

int get_mem_layout(struct memchunk *chunk_list, int size) {
  	unsigned long mSize = 0;
  	int lastRW = -1, count = 0, RW, i, pagesize, limit, *p, *start;
  	pagesize = getpagesize();
  	limit = (0xffffffff / pagesize);

  	signal(SIGSEGV, my_handler);
  
  	for (i=0; i < limit; i++) {
		/*
		 * Sets the default RW value to non readable. Sets the
		 * pointer in memory. Then performs the calls the read
		 * test and the write tests.
		 */
  	  	RW = -1;
    		p = (int*)(i * pagesize);
		RW = read_test(p);
		RW = write_test(p, RW);    
	
		if (RW == lastRW && RW != -1)
			mSize += pagesize;
   		if ((i != 0) && (RW != lastRW) && (lastRW != -1)) {
			set_chunk_list(chunk_list, start, mSize, size,
			    lastRW, count);
    		}    	
		if (RW != lastRW && RW != -1) {
			mSize = pagesize;	
			start = p;
			count++;
       		}	
    		lastRW = RW;
  	}
  	return count;
}

int read_test(int *p) {
	/*
	 * if the read does not work it will pass to the 
	 * sig handler which will jump back to the above 	
         * sigsetjump and return 1 which will jump to the
         * else where we label read_flag as 0 for fail
         */
    	if (!sigsetjmp(env, 1)) {
      		int test_read = * p;
		/*
		 * dummy operation to get rid of warning
		 */
		test_read++;
    		return 0;
    	} else {
		return -1;
	}	
}

int write_test(int *p, int RW) {	
   	/*
    	 * write test:
	 * check writeable only if the page is already 
	 * determined to be readable. if it is writeable 
	 * set RW to 1.
    	 */
    	if (RW == 0) {
      		if (!sigsetjmp(env, 1)) {
			*(int *)p = *(int *)p;
			RW = 1;
		}
    	}
	return RW;
}

void set_chunk_list(struct memchunk *chunk_list, void *start,
	unsigned long mSize, int size, int RW, int index) {
	if (index <= size) {
		chunk_list[index - 1].start = start;
		chunk_list[index - 1].length = mSize;	
		chunk_list[index - 1].RW = RW;
	}
}

void my_handler(int signo) {
  	siglongjmp(env, 1);
}
