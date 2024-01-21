OUTPUT_FORMAT(binary)
OUTPUT_ARCH(i386)
ENTRY(entry)

SECTIONS
{
	.            = 0x0000A000;
	.entry       : { *(.entry) }
	.text16      : { *(.text16) }
	.data16      : { *(.data16) }
	.            = 0x0000E000;
	.buffer16    : { *(.buffer16) }
	.            = 0x00010000;
	.text        : { *(.text) }
	.rodata      : { *(.rodata) }
	.data        : { *(.data) }
	.bss         : { *(.bss) }
}
