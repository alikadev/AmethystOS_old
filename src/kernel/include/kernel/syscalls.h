#ifndef __KERNEL__SYSCALLS_H_
#define __KERNEL__SYSCALLS_H_

#include <stdint.h>

void dispatch(void);

uint32_t _example1(void);
uint32_t _example2(void);

#endif