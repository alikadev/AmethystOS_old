#include <sys/disk.h>
#include <sys/fs/fat12.h>
#include "../disk_internal.h"

#include <string.h>

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