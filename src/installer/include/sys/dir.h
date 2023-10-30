#ifndef __SYS__DIR_H_
#define __SYS__DIR_H_

#define FILE_NAME_LEN 12

#include <sys/disk.h>

typedef enum 
{
	DIRENT_UNKNOWN = 0,
	DIRENT_DIR,
	DIRENT_FILE,
} dir_entry_type;


typedef struct
{
	dir_entry_type type; 
	char name[FILE_NAME_LEN + 1];
} dir_entry_t;

typedef struct dir_s
{
	disk_t *disk;
	dir_entry_t *entries;
	uint32_t entry_count;
} dir_t;

dir_t *dir_open(disk_t *disk, const char *dirname);
void dir_close(dir_t *dir);

#endif