#ifndef __SYS__FS__FAT12_H_
#define __SYS__FS__FAT12_H_

#include <stdint.h>

typedef struct dir_s dir_t;
typedef struct disk_s disk_t;

struct fs_fat12
{
	uint16_t bytes_per_sector;
	uint8_t sector_per_cluster;
	uint16_t reserved_sector_count;
	uint8_t fat_count;
	uint16_t max_root_entry_count;
	uint16_t sector_count;
	uint16_t sector_per_fat;
	uint16_t sector_per_track;
	uint16_t head_count;
	uint8_t boot_signature;
	uint32_t volume_id;
	char volume_label[12];
};

struct fs_fat12_header
{
	uint8_t ign0[3];
	char oem_name[8];
	uint16_t bytes_per_sector;
	uint8_t sector_per_cluster;
	uint16_t reserved_sector_count;
	uint8_t fat_count;
	uint16_t max_root_entry_count;
	uint16_t sector_count;
	uint8_t media_descriptor;
	uint16_t sector_per_fat;
	uint16_t sector_per_track;
	uint16_t head_count;
	uint32_t hidden_sector_count;
	uint32_t large_sector_count;
	uint8_t drive_number;
	uint8_t res;
	uint8_t boot_signature;
	uint32_t volume_id;
	char volume_label[11];
	char fs_type[8];
} __attribute__((packed));

typedef struct 
{
	char filename[8];
	char extension[3];
	union 
	{
		uint8_t bits;
		struct {
			uint8_t read_only : 1;
			uint8_t hidden : 1;
			uint8_t system : 1;
			uint8_t volume_label : 1;
			uint8_t subdirectory : 1;
			uint8_t archive : 1;
			uint8_t unused : 2;
		} __attribute__((packed));
	} attributes;
	uint16_t res;
	uint16_t creation_time;
	uint16_t creation_date;
	uint16_t last_access_date;
	uint16_t ign;
	uint16_t last_write_time;
	uint16_t last_write_date;
	uint16_t first_logical_cluster;
	uint32_t size;
} __attribute__((packed)) fat12_entry;

void disk_create_fat12(
	disk_t *disk, 
	struct fs_fat12_header *header);

int fat12_get_root_dir(disk_t *disk);
dir_t *fat12_dir_open(disk_t *disk, dir_t *dir, const char *dirname);
void fat12_dir_close(dir_t *dir);

#endif