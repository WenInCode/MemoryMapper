CC := gcc
CCFLAGS := -Wall -m32

memchunk:	memchunk.o test.o
	$(CC) $(CCFLAGS) memchunk.o test.o -o memchunk

test.o:		memchunk.h test.c
	$(CC) $(CCFLAGS) -c test.c -o test.o

memchunk.o:	memchunk.h memchunk.c
	$(CC) $(CCFLAGS) -c memchunk.c -o memchunk.o

clean:
	rm -rf *.o
	rm -rf memchunk
