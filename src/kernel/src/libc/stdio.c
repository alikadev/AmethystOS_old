#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <sys/io.h>

static int _curX = 0;
static int _curY = 0;
static vga_color_t _bgColor = 0x00;
static vga_color_t _fgColor = 0x0F;
static const font_t *_pFont = NULL;

void set_font(const font_t *pFont)
{
	_pFont = pFont;
}

void set_fg_color(const vga_color_t fgColor)
{
	_fgColor = fgColor;
}

void set_bg_color(const vga_color_t bgColor)
{
	_bgColor = bgColor;
}

void putchar(const int c)
{
	// Check if the font is set
	if (_pFont == NULL) return;

	// Check if the char will horizontally overflow
	if ((_curX + 1) * _pFont->width > _VGA_WIDTH)
	{
		_curX = 0;
		_curY++;
	}

	// Check if the char will vertically overflow
	if ((_curY + 1) * _pFont->height > _VGA_HEIGHT)
		_curY = 0;

	// Process the char
	switch (c)
	{
	case '\n':
		_curY++;

	case '\r':
		_curX = 0;
		break;

	default:
		vga_draw_bitmap(
				_curX * _pFont->width, 
				_curY * _pFont->height,
				_pFont->width,
				_pFont->height,
				font_get_char(_pFont, c),
				_bgColor,
				_fgColor);
		_curX++;
		break;
	}
}

void clear(void)
{
	vga_draw_rect(0,0,_VGA_WIDTH, _VGA_HEIGHT, _bgColor);
/*
	__vbuf = __VIDEO_MEMORY_START;
	while (__vbuf < __VIDEO_MEMORY_END)
	{
		*__vbuf++ = 0;
		*__vbuf++ = __vcol;
	}
	__vbuf = __VIDEO_MEMORY_START;
	x = 0;
	y = 0;*/
}

void puts(const char *s)
{
	while(*s)
	{
		putchar(*s++);
	}
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
    			putchar('%');
    		
    		else if (*format == 's' || *format == 'X')
    			puts(va_arg(args, char*));

    		else if (*format == 'c' || *format == 'C')
    			putchar(va_arg(args, int));
    		
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
    		putchar(*format);
    	}
    	format++;
    }
    va_end(args);
}

uint8_t oldkey = 0;
uint8_t getkey(void)
{/*
	while (1)
	{
		uint8_t key = inb(0x60);
		if (key == oldkey)
			continue;
		oldkey = key;
		return key;
	}*/
}

void movecur(int px, int py)
{/*
	uint16_t pos = py * __VIDEO_WIDTH + px;
	x = px;
	y = py;
 
	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t) (pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));*/
}