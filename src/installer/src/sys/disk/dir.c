#include <sys/dir.h>
#include <sys/fs/fat12.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

dir_t *dir_open(disk_t *disk, const char *dirname)
{
	dir_t *dir;

	// Check arguments
	// Sub-directories are not supported yet, 
	if (dirname != NULL)
	{
		printf("ERR: Sub-directories are not supported yet. Use NULL in dirname to read root directory");
		return NULL;
	}

	dir = malloc(sizeof *dir);
	if (!dir) return NULL;

	dir->disk = disk;

	// Read the directory
	switch (disk->type)
	{
	case FS_FAT12:
		return fat12_dir_open(disk, dir, dirname);
	default:
		printf("ERR: In dir_open, fs 0x%X is not supported\n", disk->type);
		return NULL;
	}
}

void dir_close(dir_t *dir)
{
	// Check arguments
	if (dir == NULL)
		return;

	// Read the directory
	switch (dir->disk->type)
	{
	case FS_FAT12:
		fat12_dir_close(dir);
		break;
	default:
		printf("WARN: Unknown FS %d\n");
		break;
	}

	free(dir);
}
