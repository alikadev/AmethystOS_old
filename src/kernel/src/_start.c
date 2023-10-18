#include <stdio.h>
#include <stdlib.h>
#include <sys/io.h>
#include <sys/font.h>
#include <kernel/idt.h>
#include <kernel/syscalls.h>
#include <kernel/pic.h>
#include <kernel/pci.h>

#define HEAP_START ((void*)0x1000000)
#define HEAP_SIZE  ((size_t)0x1000000)

extern font_t font_6x8;

extern uint32_t pciDeviceCount;
extern pci_device_t pciDevices[PCI_DEV_MAX_COUNT];

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

	// Initialize the PCI
	pci_init();

	// Video test
	printf("Everything is init!\n");

	for (int i = 0; i < pciDeviceCount; ++i)
	{
		uint32_t vendorID, deviceID, class, subclass, progIf;
		vendorID = pci_read(pciDevices[i], PCI_VENDOR_ID);
		deviceID = pci_read(pciDevices[i], PCI_DEVICE_ID);
		class    = pci_read(pciDevices[i], PCI_CLASS);
		subclass = pci_read(pciDevices[i], PCI_SUBCLASS);
		progIf   = pci_read(pciDevices[i], PCI_PROG_IF);

		printf("Vendor:%x, Device:%x ",
				vendorID, deviceID);
		if (class == 0xC && subclass == 0x3)
		{
			if (progIf == 0x0) printf("is UHCI");
			else if (progIf == 0x10) printf("is OHCI");
			else if (progIf == 0x20) printf("is EHCI");
			else if (progIf == 0x30) printf("is XHCI");
			else printf("is unknown USB controller");
		}

		putchar('\n');
	}
	
	while(1)
		;;
}