[bits 16]

global putc
global puts
global putx
global put32c
global put32s
global put32x
global distxtcur32
global setcur32

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
putx:
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

[bits 32]
; 
; Disable text cursor
distxtcur32:
	pushf
	push eax
	push edx
 
	mov dx, 0x3D4
	mov al, 0xA	; low cursor shape register
	out dx, al
 
	inc dx
	mov al, 0x20	; bits 6-7 unused, bit 5 disables the cursor, bits 0-4 control the cursor shape
	out dx, al
 
	pop edx
	pop eax
	popf
	ret
; 
; Set the cursor position for protected mode
; IN  AH  The row
; IN  AL  The column
setcur32:
	; Save used registers
	push    edi
	push    eax
	push    bx
	; Save AL and AH, will be poped
	push    ax
	push    ax
	; Process AL (Column)
	xor   	eax,		eax
	pop   	ax
	xor    	ah,			ah
	mov 	edi,		VIDEO_ADDR
	shl   	al,			1  			; Multiply it by 2
	add  	edi,		eax
	; Process AH (Row)
	xor     eax,		eax
	pop   	ax
	xor    	al,			al
	mov 	al,			ah			; Put AH in AL
	xor   	ah,			ah
	mov   	bx,			VIDEO_WIDTH
	mul  	bx
	shl  	eax,		1
	add  	edi,		eax
	; Put it video
	mov  	[pVideo + 0x7e00],	edi
	; Restore registers
	pop    	bx
	pop    	eax
	pop    	edi
	ret

;
; Print the CSTR referenced by ESI
put32s:
	; Save used registers
	push 	esi
	push 	ax
.fetch:
	; Load next char in AL
	lodsb
	; Check if 0 (End of string)
	or  	al, 		al
	jz  	.end
	; Print char
	call 	put32c
	jmp 	.fetch
.end:
	; Restore registers
	pop 	ax
	pop 	esi
	ret

;
; Print AL in to the screen, 0x0A is interpreted as 0x0D, 0x0A
put32c:
	cmp   	al,			0xA
	je   	put32line
	push  	edi
	mov 	edi, 		[pVideo + 0x7e00]
	mov   	[edi],		al
	mov    	[edi+1],	byte VIDEO_COLOR
	add   	edi,		2
	mov   	[pVideo + 0x7e00],	edi
	pop   	edi
	ret

;
; Print AL in to the screen, 0x0A is interpreted as 0x0D, 0x0A
put32line:
	; Save used registers
	push  	eax
	push  	ebx
	push  	edx
	; EBX is the line length in byte. Each char is 2 bytes (char + color)
	mov   	ebx,		VIDEO_WIDTH*2
	; Add LF to EAX
	mov 	eax, 		[pVideo + 0x7e00]
	add  	eax,		ebx
	push    eax
	; EDX = EAX % EBX - (0xB8000 % EBX)
	;     = EAX % EBX - 64
	div   	ebx
	sub  	edx,		64
	pop   	eax
	sub  	eax,		edx
	; Save new video pointer
	mov   	[pVideo + 0x7e00],	eax
	; Restore registers
	pop   	edx
	pop   	ebx
	pop   	eax
	ret

;
; Print AX in hexadecimal
; Source: https://github.com/ApplePy/osdev/blob/master/bootsect.asm
put32x:
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
	call 	put32c
	loop 	.next

	mov  	ax, 	0x20
	call 	put32c

	popa
	ret

section .data

VIDEO_ADDR equ 0xB8000
VIDEO_COLOR equ 0x0F
VIDEO_WIDTH equ 80
VIDEO_HEIGHT equ 25

pVideo: dd VIDEO_ADDR