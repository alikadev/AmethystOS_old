#ifndef __SYS__IO_INTERNAL_H_
#define __SYS__IO_INTERNAL_H_

#include <stdint.h>

extern int BUFFER;
#define __BUFFER ((void*)&BUFFER)
#define __BUFFER_U16 ((uint32_t)__BUFFER & 0xFFFF)
#define __BUFFER_SIZE 0x1000
#define __BUFFER_PG_SIZE (__BUFFER_SIZE / 512)

#define DISK_SECTOR_SIZE 512
#define HEAD_PER_CYLINDER
#define SECTOR_PER_TRACK


int disk_read_int13(uint8_t drive, uint16_t count, 
                    uint16_t offset, uint16_t segment, 
                    uint32_t lbalo, uint32_t lbahi);

#endif