extern putc
extern puts
extern puth

%macro  	print		1
	mov 	si,			%1
	call  	puts
%endmacro