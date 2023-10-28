#include <stdio.h>
#include <stdlib.h>
#include <sys/io.h>
#include <sys/font.h>

#define HEAP_START ((void*)0x1000000)
#define HEAP_SIZE  ((size_t)0x1000000)

extern font_t font_6x8;

uint8_t buffer[512];

void _start(uint8_t drive)
{
	uint8_t *allocBuffer;
	clear();
	set_font(&font_6x8);

	// Initialize the allocator
	alloc_init(HEAP_START, HEAP_SIZE);

	// Video test
	//printf("Installer is loaded!\n");
	//printf("Drive: 0x%02X\n", drive);

	allocBuffer = (uint8_t *)malloc(512);
	printf("Buffer [%p]\n", buffer);

	uint32_t status = disk_read(drive, allocBuffer, 1, 0);
	printf("::+3 [% 8s]\n", allocBuffer + 3);
	printf("Status: 0x%X\n", status);

	while(1)
		;;
}