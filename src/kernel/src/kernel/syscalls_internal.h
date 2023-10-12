#ifndef __KERNEL__SYSCALLS_INTERNAL_H_
#define __KERNEL__SYSCALLS_INTERNAL_H_

#define GET_EAX(var) __asm__ volatile ("movl %%eax, %0" : "=r"(i))
#define GET_EBX(var) __asm__ volatile ("movl %%ebx, %0" : "=r"(i))
#define GET_ECX(var) __asm__ volatile ("movl %%ecx, %0" : "=r"(i))
#define GET_EDX(var) __asm__ volatile ("movl %%edx, %0" : "=r"(i))

#endif