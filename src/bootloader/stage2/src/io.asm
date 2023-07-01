[bits 16]

section .text

global putc
global puts
global puth

;
; Print AL in to the screen, 0x0A is interpreted as 0x0D, 0x0A
putc:
	push  	ax 										; Save AX
	mov 	ah, 	0x0E					 		; Write character mode
	cmp 	al, 	0x0A						 	; Check if AL is LF
	je  	.line 									;  > Draw new line
.end: 
	int 	0x10									; Interrupt Video
	pop 	ax 										; Restore AX
	ret
.line:
	mov 	al,		0x0D 							; Set AL to CR
	int 	0x10
	mov 	al,		0x0A 							; Set AL to LF
	jmp 	.end 									; Jump to end

;
; Print the CSTR referenced by SI
puts:
	push 	si 										; Save SI
	push 	ax 										; Save AX
.fetch:
	lodsb 											; Load next char from SI in AL
	or  	al, 	al 								; Check if AL is 0
	jz  	.end 									;  > End
	call 	putc 									; Print char
	jmp 	.fetch 									; Fetch next
.end:
	pop 	ax 										; Restore AX
	pop 	si 										; Restore SI
	ret

;
; Print AX in hexadecimal
; Source: https://github.com/ApplePy/osdev/blob/master/bootsect.asm
puth:
	pusha
	mov  	bx, 	ax
	mov  	cx, 	4
.next:
	mov  	ax, 	bx
	rol  	bx, 	4
	shr  	ax, 	12
	cmp  	ax, 	10
	jl   	.skip
	add  	ax, 	'A'-('9'+1)
.skip:
	add  	ax, 	'0'
	call 	putc
	loop 	.next

	mov  	ax, 	0x20
	call 	putc

	popa
	ret