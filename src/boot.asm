org 0x7C00

%define ENDL 0x0D, 0x0A

_start:
	; Setup data segment
	mov AX, 0 		; Can't write directly in DS/ES
	mov DS, AX		; Set DS, 0
	mov ES, AX 		; Set ES, 0

	; Setup stack
	mov SS, AX		; Set stack to 0
	mov SP, 0 		; Set stack point to start point

	; Print HELLO message
	mov SI, msg_hello 	; Load message
	call puts		; Print the message

.main_loop: 		; Program main loop
	call getc		; Get keyboard key
	call putc		; Print the key
	jmp .main_loop	; Loop back


; Get the pressed key
; GET AH: The Scancode of the key pressed down
; GET AL: The ASCII character og the key
getc:
	mov AH, 0x00	; Read press key service
	int 0x16		; Interrupt
	ret


; Print a char in the TTY screen
; SET AL the char
putc:
	push AX			; Save AH
	mov AH, 0x0E 	; Service print TTY
	int 0x10 		; Interrupt video
	pop AX 			; Restore AH
	ret

; Print a string in the TTY screen
; SET SI: The address
puts:
	push SI 		; Save SI
	push AX 		; Save AX

.while:
	lodsb 			; Load next char in string in AL
	or AL, AL 		; Check if AL is \0
	jz .end 		; Jump to .end if process end

	mov AH, 0x0E 	; Service print TTY
	int 0x10		; Interupt video
	jmp .while

.end:
	pop AX			; Restore AX
	pop SI 			; Restore SI
	ret


msg_hello: db 'Welcome to the AmethystOS Bootloader', ENDL, 0


times 510 - ($ - $$) db 0
db 0x55, 0xAA