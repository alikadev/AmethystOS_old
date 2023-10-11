#ifndef __KERNEL__IDT_H_
#define __KERNEL__IDT_H_

#define IDT_PRESENT        (1 << 7)
#define IDT_GATE_TASK      0x5
#define IDT_GATE_INT16     0x6
#define IDT_GATE_TRAP16    0x7
#define IDT_GATE_INT32     0xE
#define IDT_GATE_TRAP32    0xF
#define IDT_RING_KERNEL    (0 << 5)
#define IDT_RING_DEVICE1   (1 << 5)
#define IDT_RING_DEVICE2   (2 << 5)
#define IDT_RING_USERSPACE (3 << 5)

void idt_init(void);
void idt_set_descriptor(uint8_t vector, void *addr, uint8_t attrs);

#endif