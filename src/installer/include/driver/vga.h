#ifndef __DRIVER__VGA_H_
#define __DRIVER__VGA_H_ 

#include <stdint.h>

#define _VGA_ADDRESS (uint8_t *)0xA0000
#define _VGA_WIDTH   320
#define _VGA_HEIGHT  200

typedef uint8_t vga_color_t;

void vga_draw_pixel(const uint16_t x,
                    const uint16_t y,
                    const vga_color_t color);

void vga_draw_rect(const uint16_t x,
                   const uint16_t y,
                   const uint16_t w,
                   const uint16_t h,
                   const vga_color_t color);

void vga_draw_bitmap(const uint16_t x,
                     const uint16_t y,
                     const uint16_t w,
                     const uint16_t h,
                     const void *bitmap,
                     const vga_color_t bgColor,
                     const vga_color_t fgColor);

#endif