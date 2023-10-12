#ifndef __IO_H_
#define __IO_H_

#include <stdint.h>
#include <sys/font.h>
#include <kernel/vga.h>

#define __DEFAULT_VIDEO_COLOR (uint8_t)(0x05)

void set_font(const font_t *pFont);
void set_fg_color(const vga_color_t fgColor);
void set_bg_color(const vga_color_t bgColor);
void putchar(const int c);
void setcol(const uint8_t color);
void puts(const char *s);
void printf(const char *format, ...);
void clear(void);
char getc(void);
void movecur(int px, int py);
uint8_t getkey(void);

#endif