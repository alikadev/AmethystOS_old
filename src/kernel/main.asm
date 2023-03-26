org 0x0
bits 16

%define CR   0x0D
%define LF   0x0A

_start:
	mov si, msg_hello 	; print hello message
	call puts

.main_loop:
	jmp .main_loop

; Print a string in the TTY screen
; IN  SI: The address
puts:
	push si 		; Save SI
	push ax 		; Save AX

.while:
	lodsb 			; Load next char in string in AL
	or al, al 		; Check if AL is \0
	jz .done 		; Jump to .end if process end
	call putc
	jmp .while

.done:
	pop ax			; Restore AX
	pop si 			; Restore SI
	ret

putc:
	push ax 		; Save AX

	mov ah, 0x0E 	; Mode print tty
	cmp al, LF 		; Check if char is LF(\n)
	je .println 	; Then print new line
	int 0x10 		; Print the char
	jmp .done 	 	; Jump to end

.println:
	mov al, CR  	; Print \r
	int 0x10
	mov al, LF 		; Print \n
	int 0x10

.done:
	pop ax 			; Restore AX
	ret

msg_hello: db 'Welcome to the AmethystOS kernel!', LF, 0