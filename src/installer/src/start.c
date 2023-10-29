#include <stdio.h>
#include <stdlib.h>
#include <sys/io.h>
#include <sys/font.h>
#include <sys/disk.h>

#define HEAP_START ((void*)0x1000000)
#define HEAP_SIZE  ((size_t)0x1000000)

extern font_t font_6x8;

void _start(uint8_t drive)
{
	disk_t *disk;
	set_font(&font_6x8);

	// Initialize the allocator
	alloc_init(HEAP_START, HEAP_SIZE);

	// Video test
	//printf("Installer is loaded!\n");
	//printf("Drive: 0x%02X\n", drive);

	disk = disk_open(drive);
	printf("Disk: %p\n", disk);

	disk_get_root_dir(disk);

	disk_close(disk);

	printf("Goodbye\n");
	while(1)
		;;
}