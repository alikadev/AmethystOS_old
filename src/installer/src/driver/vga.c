#include <driver/vga.h>
#include <stddef.h>

void vga_draw_pixel(const uint16_t x,
                    const uint16_t y,
                    const vga_color_t color)
{
	// Check if the pixel is outside of the margins
	if (x >= _VGA_WIDTH || y >= _VGA_HEIGHT) return;

	// Draw the color at the good address
	*(_VGA_ADDRESS + y * _VGA_WIDTH + x) = color;
}


void vga_draw_bitmap(const uint16_t x,
                     const uint16_t y,
                     const uint16_t w,
                     const uint16_t h,
                     const void *bitmap,
                     const vga_color_t bgColor,
                     const vga_color_t fgColor)
{
	// Get the number of bytes from the width
	uint16_t widthByteCount = w / 8;
	if (w % 8 != 0) widthByteCount++;

	// Iterate each line
	for (int yoff = 0; yoff < h; ++yoff)
	{
		// Iterate each byte of the element
		for (int byte = 0; byte < widthByteCount; ++byte)
		{
			// Get the byte from the bitmap
			uint8_t scanByte = ((uint8_t *)bitmap)[yoff * widthByteCount + byte];

			// Iterate each bit of the byte
			for (int bit = 0; bit < 8; ++bit)
			{
				// Calculate the x-offset
				int xoff = byte * 8 + bit;
				if (xoff >= w) continue;

				// Check if the bit is set
				// Bits are read from left to right
				vga_color_t color = scanByte & (1 << (7 - bit))
					? fgColor : bgColor;

				// Draw the pixel
				vga_draw_pixel(x + xoff, y + yoff, color);
			}
		}
	}
}

void vga_draw_rect(const uint16_t x,
                   const uint16_t y,
                   const uint16_t w,
                   const uint16_t h,
                   const vga_color_t color)
{
	for (int yoff = 0; yoff < h; ++yoff)
	{
		for (int xoff = 0; xoff < w; ++xoff)
		{
			vga_draw_pixel(x + xoff, y + yoff, color);
		}
	}
}