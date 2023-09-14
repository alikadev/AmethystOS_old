#include <ctype.h>

int isspace(int c)
{
	return ( c == ' '  || c == '\n' 
		  || c == '\t' || c == '\v' 
		  || c == '\f' || c == '\r');
}

int isdigit(int c)
{
	return ( c >= '0' && c <= '9');
}