#include <stdio.h>
#include <app.h>

void _start(void)
{
	clear();
	printf("Welcome in %VAmethystOS%V!\n", 0x0C, __DEFAULT_VIDEO_COLOR);
	kernel_app();
}