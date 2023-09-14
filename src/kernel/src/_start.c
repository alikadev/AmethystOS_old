#include <stdio.h>
#include <stdlib.h>

#define HEAP_START ((void*)0x1000000)
#define HEAP_SIZE  ((size_t)0x1000000)

void _start(void)
{
	clear();
	printf("Welcome in %VAmethystOS%V!\n", 0x0C, __DEFAULT_VIDEO_COLOR);
	alloc_init(HEAP_START, HEAP_SIZE);
	void *first = malloc(1);
	free(first);
	malloc(HEAP_SIZE - 1);
	void *last = malloc(1);
	printf("0x%x : 0x%x\n", first, last);
	while(1)
		;;
}