#ifndef _VIDEO_H
#define _VIDEO_H

#define _SCREEN_CHAR_WIDTH 80
#define _SCREEN_CHAR_HEIGHT 25
#define _DEFAULT_COLOR 0x04

void putc(const char c);
void puts(const char *s);
void clear(void);
void disable_cursor(void);

#endif