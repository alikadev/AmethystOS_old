#include <sys/file.h>
#include <stdio.h>
#include <stddef.h>

void *file_read(
	const disk_t *disk, 
	const char *filename)
{
	if (!disk || !filename) return NULL;

	// Read the directory
	switch (disk->type)
	{
	case FS_FAT12:
		return fat12_file_read(disk, filename);
	default:
		printf("ERR: In dir_open, fs 0x%X is not supported\n", disk->type);
		return NULL;
	}
}
