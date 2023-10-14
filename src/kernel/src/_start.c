#include <stdio.h>
#include <stdlib.h>
#include <sys/io.h>
#include <sys/font.h>
#include <kernel/idt.h>
#include <kernel/syscalls.h>
#include <kernel/pic.h>

#define HEAP_START ((void*)0x1000000)
#define HEAP_SIZE  ((size_t)0x1000000)

void testsyscalls(void);

extern font_t font_6x8;

void _start(void)
{
	clear();
	set_font(&font_6x8);

	// Remap IRQ0
	pic_remap_irq0();

	// Init IDT
	idt_init();

	// Attach the syscall interrupt code
	idt_set_descriptor(
			0xAA, dispatch, 
			IDT_PRESENT 
			| IDT_GATE_INT32 
			| IDT_RING_KERNEL);

	// Initialize the allocator
	alloc_init(HEAP_START, HEAP_SIZE);
	printf("VGA video test:\n");
	printf("  -0-1-2-3-4-5-6-7-8-9-A-B-C-D-E-F\n");
	printf("0-\n1-\n2-\n3-\n4-\n5-\n6-\n7-\n8-\n9-\nA-\nB-\nC-\nD-\nE-\nF-\n");
	for (int yoff = 0; yoff < 0x10; ++yoff)
	{
		for (int xoff = 0; xoff < 0x10; ++xoff)
		{
			vga_draw_rect(
					font_6x8.width * 2 * (xoff+1),
					font_6x8.height    * (yoff+2),
					font_6x8.width  * 2,
					font_6x8.height,
					(yoff << 4) | xoff);
		}
	}

	halt();
}