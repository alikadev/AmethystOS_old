#ifndef __SYS__IO_H_
#define __SYS__IO_H_

#include <stdint.h>

uint8_t  inb(uint16_t port);
uint16_t inw(uint16_t port);
uint32_t inl(uint16_t port);

void outb(uint16_t port, uint8_t val);
void outw(uint16_t port, uint16_t val);
void outl(uint16_t portid, uint32_t value);

void halt(void);

#endif //__SYS__IO_H_