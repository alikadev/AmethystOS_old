#include <sys/font.h>

const void *font_get_char(const font_t *pFont, const int chr)
{
	if (chr > pFont->charCount) return pFont->data;
	return pFont->data + pFont->byteWidth * pFont->height * chr;
}