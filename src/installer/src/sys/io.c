#include <sys/io.h>

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
