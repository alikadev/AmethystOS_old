#include <kernel/pic.h>
#include <sys/io.h>

//
// https://www.reddit.com/r/osdev/comments/rq3wf3/double_fault_when_enabling_interrupts_pic_apic/

 
void pic_remap_irq0(void)
{
	outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);  
	outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);

	// ICW2
	outb(PIC1_DATA, PIC1_OFFSET);
	outb(PIC2_DATA, PIC2_OFFSET);

	// ICW3
	outb(PIC1_DATA, 0x04);
	outb(PIC2_DATA, 0x02);

	// ICW4
	outb(PIC1_DATA, 0x01);
	outb(PIC2_DATA, 0x01);

	// set masks
	outb(PIC1_DATA, 0xff);
	outb(PIC2_DATA, 0xff);
}