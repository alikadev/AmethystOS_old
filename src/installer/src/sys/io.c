#include <sys/io.h>
#include "io_internal.h"

#include <string.h>

uint8_t  inb(uint16_t port)
{
    uint8_t ret;
    __asm__ volatile("inb %%dx, %%al":"=a"(ret):"d"(port));
    return ret;
}

uint16_t inw(uint16_t port)
{
    uint16_t ret;
    __asm__ volatile("inw %%dx, %%ax":"=a"(ret):"d"(port));
    return ret;
}

uint32_t inl(uint16_t port)
{
    uint32_t ret;
    __asm__ volatile("inl %%dx, %%eax":"=a"(ret):"d"(port));
    return ret;
}

void outb(uint16_t port, uint8_t val)
{
    __asm__ volatile("outb %%al, %%dx": :"d" (port), "a" (val));
}

void outw(uint16_t port, uint16_t val)
{
    __asm__ volatile("outw %%ax, %%dx": :"d" (port), "a" (val));
}

void outl(uint16_t port, uint32_t val)
{
    __asm__ volatile("outl %%eax, %%dx": :"d" (port), "a" (val));
}

int disk_read(uint8_t drive, 
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

    return 0xFF00FF00;
        
}