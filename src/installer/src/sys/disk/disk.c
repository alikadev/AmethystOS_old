#include <sys/disk.h>
#include "disk_internal.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <sys/io.h>
#include <sys/error.h>

disk_t *disk_open(uint8_t drive)
{
	return disk_create(drive);
}

void disk_close(disk_t *disk)
{
	if (disk == NULL)
	{
		_err = _ERR_INVARG;
		return;
	}

	free(disk);
}

static disk_t *disk_create(uint8_t drive)
{
	uint8_t status;
	disk_t *disk;
	union {
		char buffer[512];
		struct fs_fat12_header fat12_header;
	} data;

	// Allocate the disk
	disk = (disk_t *)malloc(sizeof *disk);
	if (!disk)
	{
		return NULL;
	}

	// Pre-fill info in the disk
	disk->id = drive;
	disk->type = FS_UNKNOWN;

	// Read the disk header (info should be there)
	status = disk_read(disk->id, data.buffer, 1, 0);
	if (status != __INT13_NO_ERROR)
		goto disk_read_failure;

 	// We know that we're using a FAT-based FS. So we're
 	// removing the spaces in the header's fs_type field
 	// to compare it.
	char *space = strchr(data.fat12_header.fs_type, ' ');
	if (space == NULL)
		goto disk_unknown_fs;

	if (space) *space = '\0';

	// Create the FS
	// TODO: Use a good way to detect the FS
	if (strcmp(data.fat12_header.fs_type, "FAT12") == 0)
		disk_create_fat12(disk, &data.fat12_header);
	else goto disk_unknown_fs;

	return disk;

disk_read_failure:
	if (status == __INT13_TIMEOUT
		|| status == __INT13_RESET_FAIL
		|| status == __INT13_UNCORRECTABLE
		|| status == __INT13_CTRL_FAILURE
		|| status == __INT13_DISK_NOT_READY)
	{
		_err = _ERR_IO;
	}
	else _err = _ERR_INTERNAL;

	goto disk_failure_finalize;

disk_unknown_fs:
	_err = _ERR_UNKNFS;
	goto disk_failure_finalize;

disk_failure_finalize:
	free(disk);
	return NULL;
}

uint8_t disk_read(uint8_t drive,
              void *ptr,
              uint16_t count,
              uint64_t lba)
{
    // Check if output address can be accessed in real mode
    if ((uint32_t)(ptr + count * DISK_SECTOR_SIZE) < (uint32_t)0xFFF0)
    {
        uint32_t ptr16 = (uint32_t)ptr & 0xFFFF;
        uint32_t lbalo = lba & 0xFFFFFFFF;
        uint32_t lbahi = (lba >> 32) & 0xFFFFFFFF;

		// try to read or return the int code
        return disk_read_int13(drive, count, ptr16, 0, lbalo, lbahi);
    }

    // Use the installer buffer in lower-memory to read on it
    // and copy it in upper memory
    int i, status;
    for (i = 0; i < count / __BUFFER_PG_SIZE; ++i, lba += 4, ptr += __BUFFER_SIZE)
    {
        uint32_t lbalo = lba & 0xFFFFFFFF;
        uint32_t lbahi = (lba >> 32) & 0xFFFFFFFF;

        // Read
        status = disk_read_int13(drive, __BUFFER_PG_SIZE, __BUFFER_U16,
                    0, lbalo, lbahi);

        // Copy to upper memory
        memcpy(ptr, __BUFFER, __BUFFER_SIZE);
    }

    // Read and copy the last sector/s
    uint16_t lastPageCount = count % __BUFFER_PG_SIZE;
    uint32_t lbalo = lba & 0xFFFFFFFF;
    uint32_t lbahi = (lba >> 32) & 0xFFFFFFFF;

    status = disk_read_int13(drive, lastPageCount, __BUFFER_U16,
            0, lbalo, lbahi);
    if (status != 0) return status;

    memcpy(ptr, __BUFFER, lastPageCount * __BUFFER_SIZE);

    return 0;
}
