#ifndef __KERNEL__IDT_INTERNAL_H_
#define __KERNEL__IDT_INTERNAL_H_ 

#include <stdint.h>

typedef struct 
{
	uint16_t addr_low;
	uint16_t segment;
	uint8_t  reserved; // Set to 0
	uint8_t  attributs;
	uint16_t addr_high;
} __attribute__((packed)) idt_entry_t;

typedef struct 
{
	uint16_t limit;
	uint32_t base;
} __attribute__((packed)) idtr_t;

#define IDT_ENTRY_COUNT 256
#define IDT_EXCEPTION_START 0
#define IDT_EXCEPTION_COUNT 32

#endif