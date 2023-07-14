OUTPUT_FORMAT(binary)
OUTPUT_ARCH(i386)
ENTRY(entry)
SECTIONS
{
	.       = 0;
	.text   : { *(.entry) *(.text) }
	.data   : { *(.data) }
	.bss    : { *(.bss) }
}