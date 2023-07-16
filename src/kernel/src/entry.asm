[bits 32]

extern _start

section .entry
global entry
entry:
	; Jump into the kernel
	mov  	edi,		0xB8000
	mov 	[edi], 		word 'k' | (0x0C << 8)
	add 	edi, 		2
	mov 	[edi], 		word 'e' | (0x06 << 8)
	add 	edi, 		2
	mov 	[edi], 		word 'r' | (0x0E << 8)
	add 	edi, 		2
	mov 	[edi], 		word 'n' | (0x02 << 8)
	add 	edi, 		2
	mov 	[edi], 		word 'e' | (0x09 << 8)
	add 	edi, 		2
	mov 	[edi], 		word 'l' | (0x05 << 8)
	jmp 	_start