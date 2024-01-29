/*OUTPUT_FORMAT(elf32-i386)*/
OUTPUT_FORMAT(binary)
OUTPUT_ARCH(i386)
ENTRY(entry)

SECTIONS
{
	/*
	 * Kernel Setup informations
	 */
	# KERNEL START @ A000
	.            = 0x0000A000;
	.entry       : { *(.entry) }

	# KERNEL 16 bits
	.            = ALIGN(32);
	.text16      : { *(.text16) }
	.data16      : { *(.data16) }
	.            = 0x0000E000;
	.buffer16    : { *(.buffer16) }

	# KERNEL 32 bits
	.            = 0x00010000;
	.text        : { *(.text) }
	.rodata      : { *(.rodata) }
	.data        : { *(.data) }

	# APPLICATION SPACE
	.            = 0x00100000;
	.app_space   : { *(.app_space) }
	.bss         : { *(.bss) }

	/*
	 * Registers protection
	 */
	.           = 0x000A0000;
	.reserved1   : { . = . + (320 * 200); }
}
