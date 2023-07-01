[bits 16]

%include "gdt.inc"
%include "io.inc"
%include "fs.inc"

section .entry

global entry
entry:
	; Save the drive id
	xor  	ah,			ah
	mov  	[diskID],	ax
	; Print stuff...
	print	s_hello							; Print greetings
	print 	s_disk_id
	call  	puth
	mov 	al,			0xA
	call 	putc
	; Setup FS
	print   s_setup_FS
	mov  	al,			[diskID]
	call  	fs_init
	jc  	failure
	; Load kernel
	print   s_searching_kernel
	mov 	si,			kernel_file
	; Kernel address is 0x1000
	mov  	bx,			0x1000
	mov  	es,			bx
	mov  	bx,			0x0000
	call    fs_read
	jc  	failure
.next:
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
	mov edi, 0x10000
	jmp edi
	hlt

section .text

failure:
	print 	s_failure
	cli
	hlt

section .data

s_hello: db "Welcome from boot.bin!", 0xA, 0
s_disk_id: db "DiskID: ", 0
s_setup_GDT: db "Setup GDT", 0xA, 0
s_setup_FS: db "Setup FS", 0xA, 0
s_searching_kernel: db "Searching and reading the kernel file", 0xA, 0
s_failure: db "Failure in finding file", 0xA, 0

kernel_file: db "KERN0   BIN"

section .bss

diskID: resb 1