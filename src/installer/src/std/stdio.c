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
}

void puts(const char *s)
{
	while(*s)
	{
		putchar(*s++);
	}
}
