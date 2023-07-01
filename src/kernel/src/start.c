#include "video.h"

void _start(void)
{
	disable_cursor();
	clear();
	puts("Hello world from the kernel!");
}