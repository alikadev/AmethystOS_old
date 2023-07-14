[bits 16]

%include "gdt.inc"
%include "io.inc"
%include "fs.inc"
%include "nmi.inc"
%include "a20.inc"

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
	mov  	[diskID],	ax
	; Print stuff...
	print	s_hello							; Print greetings
	; Setup FS
	print   s_setup_FS
	mov  	al,			[diskID]
	clc
	call  	fs_init
	jc  	failure
	; Load kernel
	print   s_searching_kernel
	mov 	si,			kernel_file
	; Kernel address is 0x1000
	mov 	bx,			0x900
	mov  	es,			bx
	xor 	bx,			bx
	clc
	call    fs_read
	jc  	failure
	mov  	edi,		0x9000-0x7e00
	mov  	ax,			word [edi]
	call  	putx
	add  	edi,		2
	mov  	ax,			word [edi]
	call  	putx
	add  	edi,		2
	mov  	ax,			word [edi]
	call  	putx
	add  	edi,		2
	mov  	ax,			word [edi]
	call  	putx
	add  	edi,		2
	mov  	ax,			word [edi]
	call  	putx
	add  	edi,		2
	mov  	ax,			word [edi]
	call  	putx
	add  	edi,		2
	mov  	ax,			word [edi]
	call  	putx
	add  	edi,		2
	mov  	ax,			word [edi]
	call  	putx
	mov  	al,			0xA
	call  	putc
	; Disable NMI
;	print   s_disable_nmi
;	call   	nmi_disable
	; Enable A20
;	print   s_enable_a20line
;	call  	a20_enable
;	cli
;	hlt
flushGDT:
	print 	s_setup_pmode
	sti
	mov ax, 0x2401
	int 0x15 ; enable A20 bit

	; Go to protected mode
	cli
	lgdt 	[gdtr]
	mov 	ax, 		0x10 ;offset to data
	mov 	ds, 		ax   ;load segment selector
	mov 	es, 		ax   ;load segment selector
	mov 	fs, 		ax   ;load segment selector
	mov 	gs, 		ax   ;load segment selector
	mov 	ss, 		ax   ;load segment selector
	mov  	eax,		cr0
	or   	eax,		1
	mov   	cr0,		eax
	jmp  	CODE_SEG:0x9000

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
s_setup_pmode: db "Setup protected mode", 0xA, 0
s_disable_nmi: db "Disable NMI", 0xA, 0
s_enable_a20line: db "Enable A20 line", 0xA, 0
s_setup_pmode_success: db "System is now in protected mode!", 0xA, 0
s_jump_kernel: db "Executing the kernel", 0xA, 0
s_failure: db "Failure in finding file", 0xA, 0

kernel_file: db "KERN0   BIN"

section .bss

diskID: resb 1
row: resb 1
col: resb 1