#include <sys/layout.h>

char keychr(__layout *layout, uint8_t keycode)
{
	return (*layout)[keycode];
}

char kb_update(__keyboard *kb, uint8_t keycode)
{
	// Check if key is function key
	char key = kb->keys[0][keycode];

	// Update the keyboard status with the new key
	if (key == __KBP_SHI)
		kb->status |= __KBMODE_SHIFT;
	else if (key == __KBR_SHI)
		kb->status &= ~__KBMODE_SHIFT;

	// Get the key
	uint8_t keymode = kb->status & __KBMODE_SHIFT;
	return kb->keys[keymode][keycode];
}