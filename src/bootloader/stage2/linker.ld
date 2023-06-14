OUTPUT_FORMAT(binary)
OUTPUT_ARCH(i386)
ENTRY(entry)
SECTIONS
{
	.       = 0x00007E00;
	.text   : { *(.text) }
	.data   : { *(.data) }
	.bss    : { *(.bss) }
}