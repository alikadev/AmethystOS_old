[org 0x7E00]
[bits 16]

; TODO: Jump in Protected Mode
; TODO: Load and run the Kernel
; TODO: Adding a Load Menu to choose what kernel to run

entry:
	mov  	si,		s_welcome
	call puts
	cli
	hlt

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
	jmp 	.end 	

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

s_welcome: db	"Welcome in the second stage", 0x0A, 0