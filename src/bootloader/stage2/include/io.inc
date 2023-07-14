extern putc
extern puts
extern putx
extern put32c
extern put32s
extern put32x
extern distxtcur32
extern setcur32

%macro  	print		1
	mov 	si,			%1
	call  	puts
%endmacro

%macro  	print32		1
	mov 	esi,		%1
	call  	puts
%endmacro