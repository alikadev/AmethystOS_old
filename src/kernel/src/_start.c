#include <stdio.h>
#include <stdlib.h>
#include <sys/io.h>
#include <kernel/idt.h>
#include <kernel/syscalls.h>

#define HEAP_START ((void*)0x1000000)
#define HEAP_SIZE  ((size_t)0x1000000)

void testsyscalls(void);

void _start(void)
{
	clear();

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

	halt();
}