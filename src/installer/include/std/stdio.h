#ifndef __STD__STDIO_H_
#define __STD__STDIO_H_

#include <stdint.h>
#include <stdarg.h>
#include <sys/font.h>
#include <driver/vga.h>

#define __DEFAULT_VIDEO_COLOR (uint8_t)(0x05)


void set_font(const font_t *pFont);
void set_fg_color(const vga_color_t fgColor);
void set_bg_color(const vga_color_t bgColor);
void putchar(const int c);
void setcol(const uint8_t color);
void puts(const char *s);
int vsprintf(char *buf, const char *fmt, va_list args);
int sprintf(char *buf, const char *fmt, ...);
int printf(const char *fmt, ...);
void clear(void);

#endif