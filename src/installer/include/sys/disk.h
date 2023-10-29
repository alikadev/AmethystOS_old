#ifndef __SYS__DISK_H_
#define __SYS__DISK_H_

#include <stdint.h>

#include <sys/fs/fat12.h>

typedef enum
{
	FS_UNKNOWN = 0,
	FS_FAT12
} fs_type;

typedef struct disk_s
{
	uint8_t id;
	fs_type type;
	union
	{
		struct fs_fat12 fat12;
	} info;
} disk_t;

disk_t *disk_open(uint8_t drive);
void disk_close(disk_t *disk);

#endif