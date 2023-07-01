OUTPUT_FORMAT(binary)
OUTPUT_ARCH(i386)
ENTRY(_start)
SECTIONS
{
	.       = 0x10000;
	.text   : { *(.entry) *(.text) }
	.data   : { *(.data) }
	.bss    : { *(.bss) }
}