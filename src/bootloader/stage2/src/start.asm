[bits 16]

extern _start

global entry
entry:
	cli
	lgdt 	[GDT_Descriptor]
	; Change last bit of CR0 to 1
	mov 	eax, 		cr0
	or 		eax,		1
	mov 	cr0,		eax
	; far jump to protected mode
	jmp 	CODE_SEGMENT:pmode 


[bits 32]
pmode:
	call _start
	cli
	hlt

GDT:
.null_segment: 	dd 0, 0 

.code_segment: 	dw 0xFFFF 		; Limit 16 bits
				dw 0 			; base 16bits
				db 0 			; base 8 bit = 24bit
				db 0b10011010 	; present,privilege,type,Type flags
				db 0b11001111 	; Other flags + Limit (last 4 bits)
				db 0 			; Last 8 bits of the base

.data_segment:	dw 0xFFFF 		; Limit
				dw 0 			; Base 16 bits
				db 0 			; Base 8 bits = 24 bits
				db 0b10010010 	; present,privilege,type,Type flags
				db 0b11001111 	; Pther flags + limit (last 4 bits)
				db 0 			; Last 8 bit of the base
.end:

GDT_Descriptor:
	dw GDT.end - GDT - 1 		; Size of the GDT
	dd GDT 						; Address of the GDT

CODE_SEGMENT equ GDT.code_segment - GDT
DATA_SEGMENT equ GDT.data_segment - GDT
