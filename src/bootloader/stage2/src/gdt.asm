[bits 16]

section .data


GDT:
.null_segment: 	dd 0
				dd 0

; Base        = 0x000000
; Limit       = 0xFFFFFF
; Access Byte = 0x9A
; Flags       = 0xC
.code_segment: 	dw 0xFFFF 		; Limit   0-15
				dw 0			; Base    0-15
				db 0 			; Base   16-23
				db 10011010b	; Access  9-7
				db 11001111b    ; Limit  16-24
				                ; Flags   0-8
				db 0            ; Base   24-31

; Base        = 0x000000
; Limit       = 0xFFFFF
; Access Byte = 0x92
; Flags       = 0xC
.data_segment: 	dw 0xFFFF 		; Limit   0-15
				dw 0  			; Base    0-15
				db 0    		; Base   16-23
				db 10010010b	; Access  9-7
				db 11001111b    ; Limit  16-24
				                ; Flags   0-8
				db 0            ; Base   24-31

.end:

global gdtr
gdtr:
	dw GDT.end - GDT - 1 		; Size of the GDT
	dd GDT + 0x7e00				; Address of the GDT

global CODE_SEG
global DATA_SEG
CODE_SEG equ GDT.code_segment - GDT
DATA_SEG equ GDT.data_segment - GDT