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
		_err = _ERR_UNKNFS;
		return NULL;
	}
}
