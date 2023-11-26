#include <stdio.h>
#include <stdlib.h>
#include <sys/io.h>
#include <sys/font.h>
#include <sys/disk.h>
#include <sys/dir.h>

#define HEAP_START ((void*)0x1000000)
#define HEAP_SIZE  ((size_t)0x1000000)

extern font_t font_6x8;

void _start(uint8_t drive)
{
	set_font(&font_6x8);

	// Initialize the allocator
	alloc_init(HEAP_START, HEAP_SIZE);

	disk_t *disk;
	
	// Open the disk
	disk = disk_open(drive);

	// Open the root directory
	dir_t *dir = dir_open(disk, "/test/test");
	if (!dir) goto nodir;

	// Read the entries
	for (int i = 0; i < dir->entry_count; i++)
	{
		printf("|- %s\n", dir->entries[i].name);
	}

	// Close everything
	dir_close(dir);
nodir:
	disk_close(disk);


	// Memory test
	void *p = malloc(1);
	if (p != HEAP_START)
		printf("Heap error: %p\n", p);
	free(p);

	while(1)
		;;
}