#ifndef __SYS__FS__FAT12_H_
#define __SYS__FS__FAT12_H_

#include <stdint.h>

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


void disk_create_fat12(
	disk_t *disk, 
	struct fs_fat12_header *header);

#endif