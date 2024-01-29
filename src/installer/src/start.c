#include <stdio.h>
#include <stdlib.h>
#include <sys/paging.h>
#include <sys/io.h>
#include <sys/font.h>
#include <sys/disk.h>
#include <sys/dir.h>
#include <sys/file.h>
#include <sys/error.h>

#define HEAP_START ((void*) 0x00100000)
#define HEAP_SIZE  ((size_t)0x000FFFFF)
extern font_t font_6x8;

void _start(uint8_t drive)
{
	set_font(&font_6x8);

	paging_init();

	// Initialize the allocator
	alloc_init(HEAP_START, HEAP_SIZE);

	disk_t *disk;

	// Open the disk
	disk = disk_open(drive);
	if (disk == NULL || _err != _ERR_NO) {
		printf("Err %d - %s: Fail to load the drive %d:\n", _err, _errstr(), drive);
		goto nodisk;
	}

	// Read file
	char *file = file_read(disk, "/test/test/file.txt");
	if (!file)
	{
		printf("Err %d - %s: Fail to read the file `/test/test/file.txt`\n", _err, _errstr());
		goto nofile;
	}
	printf("CONTENT:\n%s\n",file);

	free(file);
nofile:
	disk_close(disk);
nodisk:

	// Memory test
	void *p = malloc(1);
	if (p != HEAP_START)
		printf("Heap error: %p\n", p);
	free(p);

	while(1)
		;;
}
