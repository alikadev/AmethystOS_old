#ifndef __SYS__DISK_INTERNAL_H_
#define __SYS__DISK_INTERNAL_H_

#include <stdint.h>

extern int buffer16;
#define __BUFFER ((void*)&buffer16)
#define __BUFFER_U16 ((uint32_t)__BUFFER & 0xFFFF)
#define __BUFFER_SIZE 0x1000
#define __BUFFER_PG_SIZE (__BUFFER_SIZE / 512)

// BIOS INT13 Needed error code
#define __INT13_NO_ERROR       0x00
#define __INT13_RESET_FAIL     0x05
#define __INT13_UNCORRECTABLE  0x10
#define __INT13_CTRL_FAILURE   0x20
#define __INT13_TIMEOUT        0x40
#define __INT13_DISK_NOT_READY 0xAA

#define DISK_SECTOR_SIZE 512
#define HEAD_PER_CYLINDER
#define SECTOR_PER_TRACK

static disk_t *disk_create(uint8_t drive);

uint8_t disk_read(uint8_t drive, void *ptr,
                     uint16_t count, uint64_t lba);

uint8_t disk_read_int13(uint8_t drive, uint16_t count,
                    uint16_t offset, uint16_t segment,
                    uint32_t lbalo, uint32_t lbahi);

uint8_t disk_get_param_int13(uint8_t drive,
        uint16_t *sector_per_track, uint16_t *head_count);

void disk_status_to_error(uint8_t status);

#endif
