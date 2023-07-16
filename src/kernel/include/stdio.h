#ifndef __IO_H_
#define __IO_H_

#include <stdint.h>

#define __DEFAULT_VIDEO_COLOR (uint8_t)(0x05)

void putc(const char c);
void setcol(const uint8_t color);
void puts(const char *s);
void printf(const char *format, ...);
void clear(void);
char getc(void);
void movecur(int px, int py);
uint8_t getkey(void);

#endif