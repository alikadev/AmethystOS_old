#ifndef __KERNEL__PCI_H_
#define __KERNEL__PCI_H_


#include <stdint.h>

// Offsets
#define PCI_VENDOR_ID            0x00
#define PCI_DEVICE_ID            0x02
#define PCI_COMMAND              0x04
#define PCI_STATUS               0x06
#define PCI_REVISION_ID          0x08
#define PCI_PROG_IF              0x09
#define PCI_SUBCLASS             0x0a
#define PCI_CLASS                0x0b
#define PCI_CACHE_LINE_SIZE      0x0c
#define PCI_LATENCY_TIMER        0x0d
#define PCI_HEADER_TYPE          0x0e
#define PCI_BIST                 0x0f
#define PCI_BAR0                 0x10
#define PCI_BAR1                 0x14
#define PCI_BAR2                 0x18
#define PCI_BAR3                 0x1C
#define PCI_BAR4                 0x20
#define PCI_BAR5                 0x24
#define PCI_INTERRUPT_LINE       0x3C
#define PCI_SECONDARY_BUS        0x09

// Global informations
#define PCI_BUS_COUNT       256
#define PCI_DEV_PER_BUS     32
#define PCI_FUNC_PER_DEV    8
#define PCI_DEV_MAX_COUNT   (PCI_BUS_COUNT * PCI_DEV_PER_BUS * PCI_FUNC_PER_DEV)


typedef union {
	uint32_t bits;
	struct {
		uint32_t zero       : 2;
		uint32_t field      : 6;
		uint32_t function   : 3;
		uint32_t slot       : 5;
		uint32_t bus        : 8;
		uint32_t reserved   : 7;
		uint32_t enable     : 1;
	};
} pci_device_t;


void pci_init();
uint32_t pci_read(pci_device_t dev, uint8_t field);


#endif