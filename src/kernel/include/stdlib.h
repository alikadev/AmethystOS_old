#ifndef __STDLIB_H_
#define __STDLIB_H_

#include <stddef.h>

char *itoa(int value, char *str, int base);
int atoi(const char *str);
void alloc_init(void *start, size_t cap);
void free(void *ptr);
void *malloc(size_t size);

#endif