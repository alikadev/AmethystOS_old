[bits 16]

%include "gdt.inc"
%include "io.inc"
%include "fs.inc"

section .entry

global entry
entry:
	; Setup segments
	mov  	bx,			0x7E0
	mov  	ds,			bx
	xor  	bx,			bx
	mov  	es,			bx
	mov  	ss,			bx
	jmp  	0x07E0:.next
.next:
	; Save the drive id
	xor  	ah,			ah
	mov  	[driveID],	al
	; Print stuff...
	print	s_hello							; Print greetings
	; Setup FS
	print   s_setup_FS
	mov  	al,			[driveID]
	clc
	call  	fs_init
	jc  	failure
	; Load kernel
	print   s_searching_kernel
	mov 	si,			kernel_file
	; Kernel address is 0x1000
	mov 	bx,			(KERNEL_LOCATION >> 4)
	mov  	es,			bx
	xor  	bx,			bx
	clc
	call    fs_read
	jc  	failure
	; Setup video mode
	;   VGA 320x200 256 colors
	xor 	ah, 		ah
	mov 	al, 		0x13
	int 	0x10
flushGDT:
	print 	s_setup_pmode
	sti
	mov ax, 0x2401
	int 0x15 ; enable A20 bit

	; Go to protected mode
	cli
	lgdt 	[gdtr]
	mov  	eax,		cr0
	or   	eax,		1
	mov   	cr0,		eax
	jmp  	CODE_SEG:(pmode+0x7e00)
pmode:
	[bits 32]
	mov 	ax, 		DATA_SEG
	mov 	ds, 		ax 
	mov 	es, 		ax 
	mov 	fs, 		ax 
	mov 	gs, 		ax 
	mov 	ss, 		ax 
	mov  	ebp,		0x8FFF
	mov  	esp,		ebp

	mov 	al, 		[driveID]
	jmp  	CODE_SEG:KERNEL_LOCATION

section .text
[bits 16]
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
s_setup_pmode: db "Setup protected mode", 0xA, 0
s_disable_nmi: db "Disable NMI", 0xA, 0
s_enable_a20line: db "Enable A20 line", 0xA, 0
s_setup_pmode_success: db "System is now in protected mode!", 0xA, 0
s_jump_kernel: db "Executing the kernel", 0xA, 0
s_failure: db "Failure in finding file", 0xA, 0
kernel_file: db "INSTALL BIN"
KERNEL_LOCATION equ 0x9000

section .bss

driveID: resb 1