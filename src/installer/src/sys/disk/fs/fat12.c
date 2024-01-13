#include <sys/disk.h>
#include <sys/dir.h>
#include <sys/fs/fat12.h>
#include "../disk_internal.h"
#include "fat12_internal.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <sys/error.h>

static void fat12_clusterfilename_to_filename(
	char *dest,
	const char *src)
{
	const char *it = src;

	// Handle error
	if (dest == NULL || src == NULL)
	{
		_err = _ERR_INVARG;
		return;
	}

	// Copy the name
	for (int i = 0; i < FAT12_DIRENT_NAME_LEN; ++i)
	{
		if (*it == ' ')
			break;
		*dest++ = *it++;
	}
	it = src + FAT12_DIRENT_NAME_LEN;

	// Add the point
	if (*it != ' ')
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

static void fat12_regularfilename_to_filename(
	char *dest,
	const char *src)
{
	// Handle error
	if (dest == NULL || src == NULL)
	{
		_err = _ERR_INVARG;
		return;
	}

	// Upper the string
	while (*src)
	{
		*dest++ = toupper(*src++);
	}
	*dest = 0;
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
	{
		_err = _ERR_INVARG;
		return;
	}

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
	const fat12_entry *fat12_entry,
	dir_entry_t *entry)
{
	// Handle error
	if (fat12_entry == NULL || entry == NULL)
	{
		_err = _ERR_INVARG;
		return;
	}

	fat12_clusterfilename_to_filename(entry->name, fat12_entry->filename);
	if (fat12_entry->attributes.subdirectory)
		entry->type = DIRENT_DIR;
	else
		entry->type = DIRENT_FILE;
}




/**
 * Read the FATS and put them in a buffer
 * @param  disk The pointer to the FAT12 disk structure
 * @return      The buffer that contains the FATS
 */
static void *fat12_read_fats(
	const disk_t *disk)
{
	// Error handling
	if (!disk)
	{
		_err = _ERR_INVARG;
		return NULL;
	}

	void *fats = malloc(
		disk->info.fat12.bytes_per_sector
		* disk->info.fat12.sector_per_fat
		* disk->info.fat12.fat_count);
	if (!fats) return NULL;


	disk_read(disk->id,
	                   fats,
	                   disk->info.fat12.sector_per_fat
	                     * disk->info.fat12.fat_count,
	                   1);

	return fats;
}


/**
 * Get the next cluster to read according to the FATS
 * @param  disk    The pointer to the FAT12 disk structure
 * @param  fats    The FATS
 * @param  cluster The current cluster
 * @return         The next cluster. 0xFFF describe the end
 */
static fat12_cluster fat12_next_cluster(
	const disk_t *disk,
	const void *fats,
	const fat12_cluster cluster)
{
	// Error handling
	if (disk == NULL || fats == NULL)
	{
		_err = _ERR_INVARG;
		return 0;
	}

	// Get the next cluster
	uint32_t fat_offset = cluster + (cluster / 2); // = multiply by 1.5
	uint32_t fat_sector = 1 + (fat_offset / disk->info.fat12.bytes_per_sector);
	uint32_t ent_offset = fat_offset % disk->info.fat12.bytes_per_sector;

	int table_value = ((const char*)fats)[ent_offset];

	return (cluster & 1) ? table_value >> 4 : table_value & 0xfff;
}


/**
 * Read the rood directory and return the entries
 * @param  pOutput The pointer to the output
 * @param  disk    The pointer to the FAT12 disk structure
 * @return         FAT12_SUCCESS or the error
 */
static void fat12_read_root_directory(
	fat12_entry **pOutput,
	const disk_t *disk)
{
	int status;
	uint64_t rootdir_lba, rootdir_size;

	if (!disk || !pOutput)
	{
		_err = _ERR_INVARG;
		return;
	}

	// Calculate infos about the rootdir
	rootdir_size = 32 * disk->info.fat12.max_root_entry_count
	              / disk->info.fat12.bytes_per_sector;

	rootdir_lba = disk->info.fat12.sector_per_fat
	               * disk->info.fat12.fat_count
	               + disk->info.fat12.reserved_sector_count;

	// Reserve the entries
	*pOutput = (fat12_entry*) malloc(
		rootdir_size * disk->info.fat12.bytes_per_sector);
	if (*pOutput == NULL)
		return;

	// Read the directory
	status = disk_read(disk->id,
	                   (*pOutput),
	                   rootdir_size,
	                   rootdir_lba*0+0x13);

	if (_err != _ERR_NO) {
		free(*pOutput);
		return;
	};
}


/**
 * Transcribe a FAT cluter to a LBA address
 * @param  disk    The FAT12 disk structure
 * @param  cluster The clusterID
 * @return         The LBA address
 */
static uint64_t fat12_cluster_to_lba(
	const disk_t *disk,
	const fat12_cluster cluster)
{
	// Argument checking
	if (disk == NULL)
	{
		_err = _ERR_INVARG;
		return 0;
	}

	// Calculate the LBA address

	// rootDirSectors = ((rootEntryCount * 32) + (bytesPerSector - 1)) / bytesPerSector;
	// firstDataSector = reservedSectorCount + (fatCount * fatSize) + rootDirSectors;
	// lba = ((cluster - 2) * sectorsPerCluster) + firstDataSector;
	uint32_t rootDirSectors;
	uint32_t firstDataSector;
	uint64_t lba;

	uint16_t rootEntryCount = disk->info.fat12.max_root_entry_count;
	uint16_t bytesPerSector = disk->info.fat12.bytes_per_sector;
	uint16_t reservedSectorCount = disk->info.fat12.reserved_sector_count;
	uint8_t fatCount = disk->info.fat12.fat_count;
	uint16_t fatSize = disk->info.fat12.sector_per_fat;
	uint8_t sectorsPerCluster = disk->info.fat12.sector_per_cluster;

	rootDirSectors = ((rootEntryCount << 5) + (bytesPerSector - 1)) / bytesPerSector;
	firstDataSector = reservedSectorCount + (fatCount * fatSize) + rootDirSectors;
	lba = ((cluster - 2) * sectorsPerCluster) + firstDataSector;

	return lba;
}


/**
 * Read a cluster and all it's sub-clusters
 * @param  output The output buffer
 * @param  disk   The FAT12 disk structure
 * @param  fats   The FATs
 * @param  start  The starting cluster
 * @return        FAT12_SUCCESS or the error
 */
static void fat12_read_entry(
	void **pOutput,
	const disk_t *disk,
	const void *fats,
	const fat12_cluster start)
{
	int status;
	fat12_cluster current = start;
	uint16_t offset = 0;
	size_t size = 0;
	uint16_t bytesPerSector = disk->info.fat12.bytes_per_sector;
	uint8_t sectorPerCluster = disk->info.fat12.sector_per_cluster;
	uint64_t lba;

	// Check args
	if (!disk || !fats || start >= 0xFF8)
	{
		_err = _ERR_INVARG;
		return;
	}

	// Get size in cluster
	while (current < 0xFF8)
	{
		size++;
		current = fat12_next_cluster(disk, fats, current);
	}

	// Reserve the memory for the entry
	*pOutput = malloc(size * sectorPerCluster * bytesPerSector);
	if (_err != _ERR_NO) return;

	// Read the entry cluster by cluster
	current = start;
	while (current < 0xFF8)
	{
		lba = fat12_cluster_to_lba(disk, current);
		status = disk_read(disk->id, (*pOutput) + offset, sectorPerCluster, lba);
		if (status != 0)
		{
			free(*pOutput);
			_err = _ERR_IO;
			return;
		}
		offset += sectorPerCluster * bytesPerSector;
		current = fat12_next_cluster(disk, fats, current);
		break;
	}
}


/**
 * Tokenize a FAT12 filename and return the next element
 * @param  filename The token
 * @return          [description]
 */
static char *fat12_filename_tokenize(
	char *filename)
{
	// Check the argument
	if (!filename || !*filename)
	{
		_err = _ERR_INVARG;
		return NULL;
	}

	// Get to the next '/' or the end of the filename
	while(*filename != '/' && *filename)
		filename++;

	// Tokenization is finished!
	if (!*(filename)) return NULL;

	*filename = 0;

	return filename + 1;
}


/**
 * find an entry in a fat12 directory entry
 * @param  pEntry The pointer to the output entry
 * @param  dir    The directory to search into
 * @param  name   The entry name (filename notation)
 * @return        FAT12_SUCCESS or the error
 */
static void fat12_find_entry(
	fat12_entry **pEntry,
	fat12_entry *dir,
	const char *name)
{
	char entryName[16];

	// Check args
	if (!pEntry || !dir || !name)
	{
		_err = _ERR_INVARG;
		return;
	}

	// Iterate the directory
	for (;; ++dir)
	{
		// Check the entry status
		if (dir->filename[0] == 0)
		{
			_err = _ERR_NOTFOUND;
			return;
		}

		if (dir->filename[0] == 0xE5) continue;

		// Check the name
		fat12_clusterfilename_to_filename(entryName, dir->filename);
		if (_err != _ERR_NO) return;

		if (strcmp(entryName, name) == 0)
			break;
	}

	*pEntry = dir;
}


/**
 * Decode a filename and search through the FS to get
 * it's final entry
 * @param  pEntry   The output entry pointer (a single one)
 * @param  disk     The FAT12 disk structure
 * @param  filename The filename
 * @return          The return status
 */
static void fat12_get_entry_of_filename(
	fat12_entry **pEntry,
	const disk_t *disk,
	const void *fats,
	char *filename)
{
	fat12_entry *entries;
	fat12_entry *it;
	char *next, *current;
	fat12_cluster cluster;

	// Check args
	if (!pEntry || !disk || !filename)
	{
		_err = _ERR_INVARG;
		return;
	}

	// Read the root directory (initial directory)
	if (*filename == '/') filename++;
	fat12_read_root_directory(&entries, disk);
	if (_err != _ERR_NO)
		return;

	// Iterate entries
	current = filename;
	while (1)
	{
		next = fat12_filename_tokenize(current);
		if (_err != _ERR_NO) return;
		// Find the entry
		fat12_find_entry(&it, entries, current);
		if (_err != _ERR_NO)
		{
			_err = _ERR_NOTFOUND;
			goto fat12_find_failure;
		}

		// Check if it's the last one
		if (!next)
			break;

		current = next;

		// Check the entry
		if (!it->attributes.subdirectory)
		{
			_err = _ERR_NOTADIR;
			goto fat12_find_failure;
		}

		cluster = it->first_logical_cluster & 0xFFF;
		free(entries);
		fat12_read_entry((void**)&entries, disk, fats, cluster);
		if (_err != _ERR_NO)
			goto fat12_read_failure;
	}

	free(entries);
	*pEntry = it;
	return;


fat12_find_failure:
	free(entries);
fat12_read_failure:
}


/**
 * Read a directory and return the entries
 * @param  disk    The pointer to the FAT12 disk structure
 * @param  dirname The directory name
 * @return         The entries
 */
static void fat12_read_directory(
	fat12_entry **pEntry,
	const disk_t *disk,
	const char *dirname)
{
	char buffer[512];
	fat12_entry *entry;
	void *fats;

	// Transform the filename into usable filename
	fat12_regularfilename_to_filename(buffer, dirname);

	// Read the fats
	fats = fat12_read_fats(disk);
	if (_err != _ERR_NO)
		return;

	// Read the entry
	fat12_get_entry_of_filename(
		&entry,
		disk,
		fats,
		buffer);
	if (_err != _ERR_NO)
		return;

	if (!entry->attributes.subdirectory)
	{
		_err = _ERR_NOTADIR;
		return;
	}

	// Read the final entry
	fat12_read_entry((void**)pEntry, disk, fats, entry->first_logical_cluster & 0xFFF);

	free(fats);
}


void *fat12_file_read(
	const disk_t *disk,
	const char *filename)
{
	char aFilename[512];
	fat12_entry *entry;
	void *buffer;
	void *fats;

	// Check the arguments
	if (!disk || !filename) goto args_error;

	// Transform the filename into usable filename
	fat12_regularfilename_to_filename(aFilename, filename);
	if (_err != _ERR_NO) goto transform_error;

	// Read the fats
	fats = fat12_read_fats(disk);
	if (_err != _ERR_NO) goto fats_error;

	// Read the entry
	fat12_get_entry_of_filename(
		&entry,
		disk,
		fats,
		aFilename);
	if (_err != _ERR_NO) goto find_error;

	if (entry->attributes.subdirectory) goto not_a_file_error;

	// Read the final entry
	// Will set an error code if needed
	fat12_read_entry(&buffer, disk, fats, entry->first_logical_cluster & 0xFFF);

	free(fats);

	return buffer;

args_error:
	_err = _ERR_INVARG;
	goto error_finalize;

find_error: // Error is already set
fats_error: // Error is already set
not_a_file_error: // Error is already set
	free(fats);
transform_error: // Error is already set
error_finalize:
	return NULL;
}


/**
 * Open a FAT12 directory
 * @param  disk    The FAT12 disk
 * @param  dir     The pre-allocated directory structure (need to be close)
 * @param  dirname The directory name or NULL for the root directory
 * @return         The address of the directory structure or NULL
 */
dir_t *fat12_dir_open(
	const disk_t *disk,
	dir_t *dir,
	const char *dirname)
{
	fat12_entry *entries, *entry;
	int count = 0;

	// Check args
	if (disk == NULL
		|| disk->type != FS_FAT12
		|| dir == NULL)
	{
		_err = _ERR_INVARG;
		return NULL;
	}

	// Read the entries
	if (dirname == NULL || strcmp(dirname, "/") == 0)
		fat12_read_root_directory(&entries, disk);
	else
		fat12_read_directory(&entries, disk, dirname);

	if (_err != _ERR_NO)
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
	if (_err != _ERR_NO) return NULL;

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


void fat12_dir_close(
	dir_t *dir)
{
	free(dir->entries);
}
