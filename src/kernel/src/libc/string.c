#include <string.h>

uint32_t strlen(const char *str)
{
	uint32_t len = 0;
	while (*str++)
		len++;
}

size_t strnlen(const char *s, size_t maxlen)
{
	const char *es = s;
	while (*es && maxlen) {
		es++;
		maxlen--;
	}

	return (es - s);
}

int strcmp(const char *s1, const char *s2)
{
	while (*s1)
	{
		if (*s1 < *s2)
			return -1;
		if (*s1 > *s2)
			return 1;
		s1 ++;
		s2 ++;
	}
	if (*s2 == 0)
		return 0;
	return -1;
}