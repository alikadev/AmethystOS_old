[bits 16]

%include "gdt.inc"
%include "io.inc"

section .entry

global entry
entry:
	print	s_hello							; Print greetings
	; TODO: Load kernel.bin
	print 	s_TODO_load
	; Setup GDT
	print 	s_setup_GDT
	cli 									; Clear interrupts
	call	setupGDT						; Setup GDT
	; Enter protected mode	
	mov 	eax, 		cr0					; Get CR0
	or 		eax,		1 					; Enable protected
	mov 	cr0,		eax 				; Set CR0
	; far jump to protected mode
	jmp 	dword 		CODE_SEGMENT:.pmode ; Enter protected mode

.pmode:
	[bits 32]
	; Print x32 in magenta in screen
	mov 	edi, 		0xB8000 			
	mov 	[edi], 		word 'x' | (0x05 << 8)
	add 	edi, 		2
	mov 	[edi], 		word '3' | (0x05 << 8)
	add 	edi, 		2
	mov 	[edi], 		word '2' | (0x05 << 8)
	add 	edi, 		2
	; TODO: Jump to kernel
	hlt 									

section .data

s_hello: db "Welcome from boot.bin!", 0xA, 0
s_setup_GDT: db "Setup GDT", 0xA, 0
s_TODO_load: db "[TODO]: Load kernel.bin", 0xA, 0