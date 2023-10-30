#include <sys/disk.h>
#include <sys/dir.h>
#include <sys/fs/fat12.h>
#include "../disk_internal.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FAT12_DIRENT_NAME_LEN 8
#define FAT12_DIRENT_EXT_LEN 3

static void fat12_fatentname_to_entname(char *dest, const char *src)
{
	const char *it = src;

	// Copy the name
	for (int i = 0; i < FAT12_DIRENT_NAME_LEN; ++i)
	{
		if (*it == ' ')
			break;
		*dest++ = *it++;
	}
	it = src + FAT12_DIRENT_NAME_LEN;

	// Add the point
	*dest++ = '.';

	// Copy the extension
	for (int i = 0; i < FAT12_DIRENT_EXT_LEN; ++i)
	{
		if (*it == ' ')
			break;
		*dest++ = *it++;
	}

	// Add the zero (because asciz)
	*dest = '\0';
}

void disk_create_fat12(
	disk_t *disk, 
	struct fs_fat12_header *header)
{
	uint8_t status;
	disk->type = FS_FAT12;

	status = disk_get_param_int13(disk->id,
		&disk->info.fat12.sector_per_track,
		&disk->info.fat12.head_count);
	if (status != 0)
		return;

	// Copy the info from the header to the disk info
	disk->info.fat12.bytes_per_sector = header->bytes_per_sector;
	disk->info.fat12.sector_per_cluster = header->sector_per_cluster;
	disk->info.fat12.reserved_sector_count = header->reserved_sector_count;
	disk->info.fat12.fat_count = header->fat_count;
	disk->info.fat12.max_root_entry_count = header->max_root_entry_count;
	disk->info.fat12.sector_count = header->sector_count;
	disk->info.fat12.sector_per_fat = header->sector_per_fat;
	disk->info.fat12.boot_signature = header->boot_signature;
	disk->info.fat12.volume_id = header->volume_id;

	// Zero the first byte of the next field to copy it
	// (!!!NEED TO CHANGE IT LATER!!!)
	*header->fs_type = 0;
	strcpy(disk->info.fat12.volume_label, header->volume_label);
}

static void fat12_create_dir_entry(
		fat12_entry *fat12_entry, 
		dir_entry_t *entry)
{
	fat12_fatentname_to_entname(entry->name, fat12_entry->filename);
	if (fat12_entry->attributes.subdirectory)
		entry->type = DIRENT_DIR;
	else
		entry->type = DIRENT_FILE;
}

// int fat12_get_root_dir(disk_t *disk)
dir_t *fat12_dir_open(disk_t *disk, dir_t *dir, const char *dirname)
{
	char buffer[12] = {0};
	uint16_t lba, size;
	fat12_entry *entries, *entry;
	int status, count = 0;

	if (disk == NULL 
		|| disk->type != FS_FAT12 
		|| dir == NULL 
		|| dirname != NULL)
	{
		return NULL;
	}

	size = 32 * disk->info.fat12.max_root_entry_count 
			/ disk->info.fat12.bytes_per_sector;

	lba = disk->info.fat12.sector_per_fat 
			* disk->info.fat12.fat_count 
			+ disk->info.fat12.reserved_sector_count;

	// Reserve the directories buffer
	entries = (fat12_entry *)malloc(size * disk->info.fat12.bytes_per_sector);
	if (!entries) return NULL;

	// Read the directories
	status = disk_read(disk->id, entries, 1, lba);
	if (status != 0) 
		return NULL;

	// Count entries
	for (entry = entries;; ++entry)
	{
		// If filename = 0, all next are free
		if (entry->filename[0] == 0)
			break;

		// If filename = 0xE5, this one is free
		if (entry->filename[0] == 0xE5)
			continue;

		// Do not count the volume label
		if (!entry->attributes.volume_label)
			count++;
	}

	// Allocate the entries
	dir->entry_count = count;
	dir->entries = (dir_entry_t *)malloc(count * sizeof *dir->entries);
	
	// Copy the entries
	count = 0;
	for (entry = entries;; ++entry)
	{
		// If filename = 0, all next are free
		if (entry->filename[0] == 0)
			break;

		// If filename = 0xE5, this one is free
		if (entry->filename[0] == 0xE5)
			continue;

		// Do not count the volume label
		if (!entry->attributes.volume_label)
			fat12_create_dir_entry(entry, &dir->entries[count++]);
	}

	// The used memory
	free(entries);

	return dir;
}

void fat12_dir_close(dir_t *dir)
{
	free(dir->entries);
}