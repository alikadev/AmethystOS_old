#include "syscalls_internal.h"
#include <kernel/syscalls.h>
#include <stdio.h>

void *syscall_list[] =
{
	_example1, // 0
	_example2  // 1
};

uint32_t _example1(void)
{
	printf("Example 1 has been called!\n");
}

uint32_t _example2(void)
{
	uint32_t i;
	GET_EBX(i);
	printf("Example 2 has been called and the argument is %d!\n", i);
}
