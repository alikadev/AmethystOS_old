#include <stdio.h>
#include <stdlib.h>
#include <sys/io.h>
#include <sys/font.h>

#define HEAP_START ((void*)0x1000000)
#define HEAP_SIZE  ((size_t)0x1000000)

extern font_t font_6x8;

void _start(uint8_t drive)
{
	clear();
	set_font(&font_6x8);

	// Initialize the allocator
	alloc_init(HEAP_START, HEAP_SIZE);

	// Video test
	printf("Installer is loaded!\n");
	printf("drive: 0x%02X\n", drive);

	while(1)
		;;
}