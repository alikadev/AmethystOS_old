OUTPUT_FORMAT(binary)
OUTPUT_ARCH(i386)
ENTRY(entry)

SECTIONS
{
	.            = 0x00009000;
	.entry       : { *(.entry) }
	.            = 0x0000A000;
	.buffer : 
	{
		*(.buffer)
		BUFFER = .;
	}
	.        = 0x0000C000;
	.text        : { *(.text) }
	.rodata      : { *(.rodata) }
	.data        : { *(.data) }
	.bss         : { *(.bss) }
}