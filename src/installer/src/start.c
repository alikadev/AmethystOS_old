#include <stdio.h>
#include <stdlib.h>
#include <sys/io.h>
#include <sys/font.h>
#include <sys/disk.h>
#include <sys/dir.h>

#define HEAP_START ((void*)0x1000000)
#define HEAP_SIZE  ((size_t)0x1000000)
#define IT_COUNT   100
extern font_t font_6x8;

typedef struct 
{
	float x;
	float y;
} vec2f;

typedef struct 
{
	int x;
	int y;
} vec2i;

void compute_next(vec2f *next, vec2f current, vec2f constant)
{
	next->x = current.x * current.x - current.y * current.y + constant.x;
	next->y = 2.f * current.x * current.y + constant.y;
}

float mod2(vec2f z)
{
	return z.x * z.x + z.y * z.y;
}

int compute_it(vec2f z0, vec2f constant, int max_it)
{
	int it = 0;

	while(mod2(z0) < 4.f && it < max_it)
	{
		compute_next(&z0, z0, constant);
		it++;
	}

	return it;
}

void render(vec2i pos, vec2i size, vec2f constant, float zoom)
{
	const float scale = 1.f / ((float)zoom / 2.f);

	for (int y = 0; y < size.y; ++y)
	{
		for (int x = 0; x < size.x; ++x)
		{
			const float px = ((float)x - (float)size.x / 2.f) * scale;
			const float py = ((float)y - (float)size.y / 2.f) * scale;
			int it = compute_it((vec2f){px, py}, constant, IT_COUNT);
			uint8_t color = it * 0xF / IT_COUNT + 0x10;
			vga_draw_pixel(x+pos.x, y+pos.y, color);
		}
	}
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

void _start(uint8_t drive)
{
	set_font(&font_6x8);

	// Initialize the allocator
	alloc_init(HEAP_START, HEAP_SIZE);

	getkey();

	printf("This is a fractal simulation. Old CPUs can have a hard time updating it.\n");
	printf("\nPress any key to start.\n");
	getkey();
	vec2f v;
	v.x = 0;
	v.y = 0;
	float zoom = 250.f;
	vec2f constant = {-0.35, 0.65};
	float mx = -0.001, my = 0.001;
	float max_const = 1.f;
	float min_const = -1.f;
	while(1)
	{
		render((vec2i){0,0},
		       (vec2i){_VGA_WIDTH, _VGA_HEIGHT},
		       constant, 
		       zoom);
		constant.x += mx;
		constant.y += my;
		if (constant.x > max_const || constant.x < min_const)
			mx *= -1;
		if (constant.y > max_const || constant.y < min_const)
			my *= -1;
	}
	while(1)
		;;
}