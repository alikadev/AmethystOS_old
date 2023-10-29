#include <sys/disk.h>
#include "disk_internal.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <sys/io.h>

disk_t *disk_open(uint8_t drive)
{
	disk_t *disk = disk_create(drive);
	if (!disk)
		return NULL;
	if (disk->type == FS_UNKNOWN)
	{
		disk_close(disk);
		return NULL;
	}
	return disk;
}

void disk_close(disk_t *disk)
{
	free(disk);
}

disk_t *disk_create(uint8_t drive)
{
	disk_t *disk;
	union {
		char buffer[512];
		struct fs_fat12_header fat12_header;
	} data;

	// Allocate the disk	
	disk = (disk_t *)malloc(sizeof *disk);
	if (!disk) {
		printf("NO ALLOC\n");
		return NULL;
	}

	// Pre-fill info in the disk
	disk->id = drive;
	disk->type = FS_UNKNOWN;

	__asm__("push %eax");
	__asm__("mov %eax, %ebx");
	__asm__("mov %eax, %ebx");
	__asm__("mov %eax, %ebx");
	__asm__("pop %eax");

	// Read the disk header (info should be there)
	disk_read(disk->id, data.buffer, 1, 0);

 	// We know that we're using a FAT-based FS. So we're
 	// removing the spaces in the header's fs_type field
 	// to compare it.
	char *space = strchr(data.fat12_header.fs_type, ' ');
	if (space) *space = '\0';

	// Create the FS
	if (strcmp(data.fat12_header.fs_type, "FAT12") == 0) 
		disk_create_fat12(disk, &data.fat12_header);

	return disk;
}