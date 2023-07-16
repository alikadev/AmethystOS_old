#ifndef __SYS__LAYOUT_H_
#define __SYS__LAYOUT_H_

#include <stdint.h>

#define __KBMODE_SHIFT 		(1 << 0)

typedef uint8_t __layout[256];

typedef struct
{
	uint32_t id;
	
	char name[32];

	uint8_t status;
	__layout keys[2];
} __keyboard;


char keychr(__layout *layout, uint8_t keycode);

char kb_update(__keyboard *layout, uint8_t keycode);

#define __KBP_SHI 1
#define __KBR_SHI 2
#define SHI __KBP_SHI   // SHIFT-LEFT
#define NSHI __KBR_SHI   // NO SHIFT-LEFT
#define ESC '\e' // ESCAPE
#define CLF '\n' // CHARACTER LINE FEED
#define DEL '\b'

#endif