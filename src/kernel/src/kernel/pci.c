#include <kernel/pci.h>

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <sys/io.h>


// PCI informations
#define PCI_VENDOR_NONE  0xFFFF

// Ports
#define PCI_CONFIG_ADDRESS  0xCF8
#define PCI_CONFIG_DATA     0xCFC


static uint8_t pci_size_map[] = 
{
	[PCI_VENDOR_ID]         = 2,
	[PCI_DEVICE_ID]         = 2,
	[PCI_COMMAND]           = 2,
	[PCI_STATUS]            = 2,
	[PCI_REVISION_ID]       = 1,
	[PCI_PROG_IF]           = 1,
	[PCI_SUBCLASS]          = 1,
	[PCI_CLASS]             = 1,
	[PCI_CACHE_LINE_SIZE]   = 1,
	[PCI_LATENCY_TIMER]     = 1,
	[PCI_HEADER_TYPE]       = 1,
	[PCI_BIST]              = 1,
	[PCI_BAR0]              = 4,
	[PCI_BAR1]              = 4,
	[PCI_BAR2]              = 4,
	[PCI_BAR3]              = 4,
	[PCI_BAR4]              = 4,
	[PCI_BAR5]              = 4,
	[PCI_INTERRUPT_LINE]    = 1,
	[PCI_SECONDARY_BUS]     = 1,
};


uint32_t pciDeviceCount = 0;
pci_device_t pciDevices[PCI_DEV_MAX_COUNT];


uint32_t pci_read(
		pci_device_t dev,
		uint8_t field)
{
	dev.enable = 1;
	dev.field = (field & 0xFC) >> 2;

	outl(PCI_CONFIG_ADDRESS, dev.bits);

	uint32_t data = inl(PCI_CONFIG_DATA);

	switch (pci_size_map[field])
	{
	case 1: return data >> ((field & 3) * 8) & 0xFF;
	case 2: return data >> ((field & 2) * 8) & 0xFFFF;
	case 4: return data; 
	default:
		printf("pci_read field %d (0x%x) is not supported.");
		halt();
	}
}

static inline
void pci_add_device(pci_device_t dev)
{
	pciDevices[pciDeviceCount].bits = dev.bits;
	pciDeviceCount ++;
}

static
void pci_scan(void)
{
	// Clear the PCI devices
	pciDeviceCount = 0;

	pci_device_t device;
	for (int bus = 0; bus < PCI_BUS_COUNT; ++bus)
	{
		device.bus = bus;
		for (int slot = 0; slot < PCI_DEV_PER_BUS; ++slot)
		{
			device.slot = slot;
			for (int fn = 0; fn < PCI_FUNC_PER_DEV; ++fn)
			{
				device.function = fn;
				if (pci_read(device, PCI_VENDOR_ID) == PCI_VENDOR_NONE)
					continue;
				pci_add_device(device);
			}
		}
	}
}

void pci_init(void)
{
	// Searching all the devices
	pci_scan();
}