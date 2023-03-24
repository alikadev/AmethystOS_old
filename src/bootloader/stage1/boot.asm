[org 0x7C00]
[bits 16]

_boot:
	mov 	[diskid],	dl 			; Save disk drive number

	mov  	si, 	msg_welcome 	; Print greetings
	call 	puts

	call 	read

	jmp 	halt 					; Halt

read:
	mov 	al, 	1 				; Number of sector to be read
	mov 	ch, 	0 				; Cylinder number
	mov 	cl, 	2 				; Sector number
	mov 	dh, 	0 				; Head number
	mov 	dl, 	[diskid] 		; Hard disk number
	mov  	bx,		0 				; ES:BX -> Address of
	mov 	es, 	bx 				; pointer
	mov 	bx, 	0x7E00 			; BX is 7E00
	
	mov		ah, 	2 				; Disk interrupt
	int 	0x13 					; Read sector
	jc 		.error 					; If carry is set -> Error
.end:
	mov 	ax, 	[0x7E00] 		; Read byte
	call 	puth 					; Print
	ret
.error:
	mov 	al,		ah 				; Set error code in AL to be printed
	mov  	si, 	err_disk_read	; Set message read failure in SI
	call 	puts 					; Print the message
	call 	puth 					; Print the error code
	jmp 	halt 					; Halt the machine

;
; Print AL in to the screen, 0x0A is interpreted as 0x0D, 0x0A
putc:
	push  	ax 					; Save AX
	mov 	ah, 	0x0E 		; Write character mode
	cmp 	al, 	0x0A	 	; Check if AL is LF
	je  	.line 				;  > Draw new line
.end: 
	int 	0x10				; Interrupt Video
	pop 	ax 					; Restore AX
	ret
.line:
	mov 	al,		0x0D 		; Set AL to CR
	int 	0x10
	mov 	al,		0x0A 		; Set AL to LF
	jmp 	.end 				; Jump to end


;
; Print the CSTR referenced by SI
puts:
	push 	si 						; Save SI
	push 	ax 						; Save AX
.fetch:
	lodsb 							; Load next char from SI in AL
	or  	al, 	al 				; Check if AL is 0
	jz  	.end 					;  > End
	call 	putc 					; Print char
	jmp 	.fetch 					; Fetch next
.end:
	pop 	ax 						; Restore AX
	pop 	si 						; Restore SI
	ret
;
; Print AX in hexadecimal
; (FROM https://github.com/ApplePy/osdev/blob/master/bootsect.asm)
puth:
	push 	bx 				
	push 	cx
	mov  	bx, 	ax
	mov  	cx, 	4
.next:
	mov  	ax, 	bx
	rol  	bx, 	4
	shr  	ax, 	12
	cmp  	ax, 	10
	jl   	.skip
	add  	ax, 	7
.skip:
	add  	ax, 	48
	call 	putc
	loop 	.next

	pop  	cx
	pop  	bx

	mov  	ax, 	0x20
	call 	putc

	ret


diskid: dw 0

halt:
	cli
	hlt

msg_welcome: 		db "Booted", 0x0A, 0
err_disk_read: 		db "F:READ=", 0

times 510 - ($ - $$) db 0
db 0x55, 0xAA
