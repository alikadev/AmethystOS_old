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
		pci_device_t device = pciDevices[i];
		vendorID = pci_read(device, PCI_VENDOR_ID);
		deviceID = pci_read(device, PCI_DEVICE_ID);
		class    = pci_read(device, PCI_CLASS);
		subclass = pci_read(device, PCI_SUBCLASS);
		progIf   = pci_read(device, PCI_PROG_IF);

		printf("Vendor:%04X, Device:%04X, ",
				vendorID, deviceID, class, subclass);
		if (class == 0xC && subclass == 0x3)
		{
			uint32_t bar0;
			bar0 = pci_read(device, PCI_BAR0);
			if (progIf == 0x20)
			{
				printf("EHCI:%08X\n", bar0);
			}
			else if (progIf == 0x30)
			{
				uint32_t bar1;
				bar1 = pci_read(device, PCI_BAR1);

				printf("XHCI:%08X.%08X\n", bar1, bar0);
			}
		} 
		else printf("Cl: %02X-%02X\n", class, subclass);
	}
	
	while(1)
		;;
}