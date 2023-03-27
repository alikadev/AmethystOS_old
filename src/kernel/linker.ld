OUTPUT_ARCH(i386)
OUTPUT_FORMAT(binary)
ENTRY(_start)
SECTIONS
{
  .text.boot : { *(.text.boot) }
  .text : { *(.text) }
  .data : { *(.data) }
}