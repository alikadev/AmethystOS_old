#ifndef __SYS__FONT_H_
#define __SYS__FONT_H_

#include <stdint.h>

typedef const uint8_t font_data[];

typedef struct 
{
	const char *name;
	uint16_t width;
	uint16_t height;
	uint16_t byteWidth;
	uint16_t charCount;
	const void *data;
} font_t;

const void *font_get_char(const font_t *pFont, const int chr);

#endif