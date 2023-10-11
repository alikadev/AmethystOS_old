#ifndef __SYS__IO_H_
#define __SYS__IO_H_

#include <stdint.h>


void outb(uint16_t port, uint8_t val);
uint8_t inb(uint16_t port);
void outw(uint16_t port, uint16_t val);
uint16_t inw(uint16_t port);

void halt(void);

#endif //__SYS__IO_H_