#include <app.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/io.h>
#include <sys/layout.h>
#include <string.h>

#define INPUT_LEN 64

#define MODE_KEYCODE    (1 << 0)
#define MODE_DEBUG      (1 << 1)

char mode=0;
char input[INPUT_LEN] = {0};
char pinput = 0;

__keyboard keyboard_fr_ch = {
	.id = 0,
	.name = "FR-CH GENERIC",
	.keys = {
		{
			 0 ,   ESC,   '1',   '2',   '3',   '4',   '5',   '6',   '7',   '8',   '9',   '0',  '\'',   '^', 
			DEL,    0 ,   'q',   'w',   'e',   'r',   't',   'z',   'u',   'i',   'o',   'p',   'e',   '"', 
			CLF,    0 ,   'a',   's',	'd',   'f',   'g',   'h',   'j',   'k',   'l',    0 ,   'a',    0 , 
			SHI,   '$',   'y',   'x',   'c',   'v',   'b',   'n',   'm',   ',',   '.',   '-',   SHI,    0 ,
			 0 ,   ' ',    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,
			 0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,
			 0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,
			 0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,
			 0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,
			 0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,
			 0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,
			 0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,
			 0 ,    0 ,  NSHI,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,
		     0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,
			 0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,
			 0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,
			 0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,
			 0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,
			 0 ,    0 ,    0 ,    0
		}, {
			 0 ,   ESC,   '+',   '"',   '*',   'c',   '%',   '&',   '/',   '(',   ')',   '=',   '?',   '~', 
			DEL,    0 ,   'Q',   'W',   'E',   'R',   'T',   'Z',   'U',   'I',   'O',   'P',   'u',   '!', 
			CLF,    0 ,   'A',   'S',	'D',   'F',   'G',   'H',   'J',   'K',   'L',    0 ,   'o',    0 , 
			SHI,   '$',   'Y',   'X',   'C',   'V',   'B',   'N',   'M',   ';',   ':',   '_',   SHI,    0 ,
			 0 ,   ' ',    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,
			 0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,
			 0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,
			 0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,
			 0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,
			 0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,
			 0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,
			 0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,
			 0 ,    0 ,  NSHI,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,
		     0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,
			 0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,
			 0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,
			 0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,
			 0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,    0 ,
			 0 ,    0 ,    0 ,    0
		}
	}
};


char *toknext(char **str, char delim)
{
	char *start = *str;
	while(**str)
	{
		if(**str == delim)
		{
			**str = 0;
			(*str)++;
			return start;
		}
		(*str)++;
	}
	return start;
}

void process(char *request)
{
	if(!*request)
	{
		return;
	}
	setcol(0x0C);
	char *origin = request;
	if(mode & MODE_DEBUG) printf("Req: [%s]\n", origin);
	char *cmd = toknext(&request, ' ');
	if (strcmp(cmd, "help") == 0)
	{
		printf("Keyboard shortcuts:\n");
		printf("  ESCAPE     Clear screen\n");
		printf("Commands:\n");
		printf("  help       Print this\n");
		printf("  clear      Clear the screen\n");
		printf("  echo       Print the next string\n");
		printf("  keycode    Start KEYCODE mode\n");
		printf("  nokeycode  Exit KEYCODE mode\n");
		printf("  debug      Start DEBUG mode\n");
		printf("  nodebug    Exit DEBUG mode\n");
	}
	else if (strcmp(cmd, "keycode") == 0)
	{
		printf("KEYCODE MODE ENABLED\n");
		mode |= MODE_KEYCODE;
	}
	else if (strcmp(cmd, "nokeycode") == 0)
	{
		printf("KEYCODE MODE DISABLED\n");
		mode &= ~MODE_KEYCODE;
	}
	else if (strcmp(cmd, "debug") == 0)
	{
		printf("DEBUG MODE ENABLED\n");
		mode |= MODE_DEBUG;
	}
	else if (strcmp(cmd, "nodebug") == 0)
	{
		printf("DEBUG MODE ENABLED\n");
		mode &= ~MODE_DEBUG;
	}
	else if (strcmp(cmd, "echo") == 0)
	{
		printf("%s\n", request);
	}
	else if (strcmp(cmd, "clear") == 0)
	{
		setcol(__DEFAULT_VIDEO_COLOR);
		clear();
	}
	else
		printf("Unknown command [%s]. Write [help] to print help message\n", cmd);
	setcol(__DEFAULT_VIDEO_COLOR);
}

void kernel_app(void)
{
	putc('>');
	while(1)
	{
		uint8_t key = getkey();
		char chr = kb_update(&keyboard_fr_ch, key);

		if (mode & MODE_KEYCODE) printf("[%x] => ", key);

		switch (chr)
		{
		case 0:
			if (mode & MODE_KEYCODE) printf("[?]\n");
			continue;
		case SHI:
			if (mode & MODE_KEYCODE) printf("[SHI]\n");
			continue;
		case NSHI:
			if (mode & MODE_KEYCODE) printf("[NSHI]\n");
			continue;
		case ESC:
			clear();
			if(mode & MODE_KEYCODE) printf("[%x] => [ESC]\n", key);
			if (!(mode & MODE_KEYCODE)) putc('>');
			puts(input);
			continue;
		case CLF:
			if(mode & MODE_KEYCODE) printf("[CLF]");
			putc('\n');
			input[pinput] = 0;
			pinput = 0;
			process(input);
			input[pinput] = 0;
			puts(">");
			continue;
		case DEL:
			if (pinput != 0)
			{
				pinput--;
				if (!(mode & MODE_KEYCODE)) putc('\b');
			}
			if (mode & MODE_KEYCODE) printf("DEL\n");
			continue;
		default:
			input[pinput++] = chr;
			putc(chr);
			if(mode & MODE_KEYCODE) putc('\n');
		}
	}
}