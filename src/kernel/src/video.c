#include "video.h"
#include "stdint.h"

#define _VIDEO_MEMORY 0xB8000

volatile uint8_t *vmem = (volatile char *)_VIDEO_MEMORY;

void puts(const char *s)
{
	while (*s)
	{
		*vmem++ = *s++;
		*vmem++ = _DEFAULT_COLOR;
	}
}

void putc(const char c)
{
	*vmem++ = c;
	*vmem++ = _DEFAULT_COLOR;
}

void clear(void)
{
	volatile uint16_t *vmem16 = (volatile uint16_t *)_VIDEO_MEMORY;
	vmem = (volatile uint8_t *)vmem16;
	for (uint64_t i = 0; i < _SCREEN_CHAR_HEIGHT * _SCREEN_CHAR_WIDTH; ++ i)
	{
		*vmem16++ = _DEFAULT_COLOR << 8;
	}
}