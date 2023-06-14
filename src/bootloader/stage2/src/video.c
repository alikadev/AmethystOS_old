#include "video.h"
#include "stdint.h"

volatile uint8_t *vmem = (volatile char *)0xB8000;

void puts(const char *s)
{
	while (*s)
	{
		*vmem++ = *s++;
		*vmem++ = 0x8F;
	}
}

void putc(const char c)
{
	*vmem++ = c;
	*vmem++ = 0x8F;
}

void clear(void)
{
	volatile uint16_t *vmem16 = (volatile uint16_t *)0xB8000;
	vmem = (volatile uint8_t *)0xB8000;
	for (uint64_t i = 0; i < _SCREEN_CHAR_HEIGHT * _SCREEN_CHAR_WIDTH; ++ i)
	{
		*vmem16++ = 0x8F00;
	}
}