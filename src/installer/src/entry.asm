[bits 32]

extern _start

section .entry
global entry
entry:
	; Jump into the kernel
	push  	dx
	call 	_start
	cli
	hlt