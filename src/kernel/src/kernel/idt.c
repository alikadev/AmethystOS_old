#include "idt_internal.h"
#include <kernel/idt.h>
#include <kernel/isr.h>
#include <stdio.h>

__attribute__((aligned(0x10)))
static idt_entry_t _idt[IDT_ENTRY_COUNT] = {0};

static idtr_t _idtr;

void idt_init(void)
{
	_idtr.base = (uintptr_t)&_idt[0];
	_idtr.limit = (uint16_t) sizeof(idt_entry_t) * IDT_ENTRY_COUNT - 1;

	__asm__ volatile ("lidt %0" : : "m"(_idtr));
	__asm__ volatile ("sti");

	for (int i = IDT_EXCEPTION_START; i < IDT_EXCEPTION_COUNT; i++)
	{
		idt_set_descriptor(
				i, 
				isr_undefined_error, 
				IDT_PRESENT | IDT_GATE_INT32 | IDT_RING_KERNEL);
	}
}

void idt_set_descriptor(uint8_t vector, void *addr, uint8_t attrs)
{
	_idt[vector].addr_low = (uint32_t) addr & 0xFFFF;
	_idt[vector].addr_high = (uint32_t) addr >> 16;
	_idt[vector].segment = 0x8;
	_idt[vector].attributs = attrs;
	_idt[vector].reserved = 0;
}