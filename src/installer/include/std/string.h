#ifndef __STRING_H_
#define __STRING_H_

#include <stdint.h>
#include <stddef.h>

uint32_t strlen(const char *str);
size_t strnlen(const char *s, size_t maxlen);
int strcmp(const char *s1, const char *s2);
void *memcpy(void* dest, const void* src, size_t count);
char *strchr(const char *p, int ch);
char* strcpy(char* dest, const char* src);

#endif