#include <stdio.h>
#include <sys/io.h>
#include <kernel/isr.h>

void isr_undefined_error(void)
{
	printf("Undefined error, halt");
	halt();
}