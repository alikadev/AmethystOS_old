#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <sys/io.h>

#define __VIDEO_WIDTH 80
#define __VIDEO_HEIGHT 25
#define __VIDEO_CHAR_SIZE 2
#define __VIDEO_MEMORY_START (uint8_t*)0xb8000
#define __VIDEO_MEMORY_SIZE (uint32_t)(__VIDEO_WIDTH * __VIDEO_HEIGHT * __VIDEO_CHAR_SIZE)
#define __VIDEO_MEMORY_END (__VIDEO_MEMORY_START + __VIDEO_MEMORY_SIZE)

volatile static uint8_t *__vbuf = __VIDEO_MEMORY_START;
volatile static uint8_t __vcol = __DEFAULT_VIDEO_COLOR;
volatile static int x = 0;
volatile static int y = 0;

void printchar(const char c)
{
	if (__vbuf >= __VIDEO_MEMORY_END)
	{
		__vbuf = __VIDEO_MEMORY_START;
		x = 0;
		y = 0;
	}

	switch (c)
	{
	case '\n':
		__vbuf += __VIDEO_WIDTH * __VIDEO_CHAR_SIZE;
		y ++;
	case '\r':
		uint32_t tmp = (uint32_t)__vbuf;
		tmp -= tmp % (__VIDEO_WIDTH * __VIDEO_CHAR_SIZE) + 96;
		__vbuf = (volatile uint8_t *)tmp;
		x = 0;
		break;
	case '\x8':
		if (x == 0) return;
		__vbuf -= 2;
		*__vbuf = ' ';
		x --;
		break;
	default:
		*__vbuf++ = c;
		*__vbuf++ = __vcol;
		x ++;
		if(x >= __VIDEO_WIDTH)
		{
			x -= __VIDEO_WIDTH;
			y ++;
		}
	}
}

void clear(void)
{
	__vbuf = __VIDEO_MEMORY_START;
	while (__vbuf < __VIDEO_MEMORY_END)
	{
		*__vbuf++ = 0;
		*__vbuf++ = __vcol;
	}
	__vbuf = __VIDEO_MEMORY_START;
	x = 0;
	y = 0;
}

void putc(const char c)
{
	printchar(c);
	movecur(x,y);
}

void setcol(const uint8_t color)
{
	__vcol = color;
}

void puts(const char *s)
{
	while(*s)
	{
		printchar(*s++);
	}
	movecur(x,y);
}

void printf(const char *format, ...)
{
    va_list args;
    char sbuf[33];

    va_start(args, format);
    while(*format)
    {
    	if(*format == '%')
    	{
    		format++;
    		if (*format == '%')
    			putc('%');
    		else if (*format == 'V' || *format == 'v')
    			setcol(va_arg(args, int));
    		else if (*format == 's' || *format == 'X')
    			puts(va_arg(args, char*));
    		else if (*format == 'c' || *format == 'C')
    			putc(va_arg(args, int));
    		else if (*format == 'd' || *format == 'D')
    		{
    			itoa(va_arg(args, int), sbuf, 10);
    			puts(sbuf);
    		}
    		else if (*format == 'x' || *format == 'X')
    		{
    			itoa(va_arg(args, int), sbuf, 16);
    			puts(sbuf);
    		}
    		else if (*format == '\0')
    			return;
    	}
    	else
    	{
    		putc(*format);
    	}
    	format++;
    }
    va_end(args);
}

uint8_t oldkey = 0;
uint8_t getkey(void)
{
	while (1)
	{
		uint8_t key = inb(0x60);
		if (key == oldkey)
			continue;
		oldkey = key;
		return key;
	}
}

void movecur(int px, int py)
{
	uint16_t pos = py * __VIDEO_WIDTH + px;
	x = px;
	y = py;
 
	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t) (pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}