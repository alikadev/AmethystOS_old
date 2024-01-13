#include <sys/font.h>
#include <stddef.h>
#include <sys/error.h>

const void *font_get_char(const font_t *pFont, const int chr)
{
	if (pFont == NULL)
	{
		_err = _ERR_INVARG;
		return NULL;
	}

	if (chr > pFont->charCount) return pFont->data;
	return pFont->data + pFont->byteWidth * pFont->height * chr;
}
