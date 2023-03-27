bits 16
section .text

%include "basicio.inc"

; Get a string from the user
; @param di The buffer
; @param ax The size
gets:
    push di         ; Save DI (The pointer)
    push ax         ; Save AX
    push bx         ; Save BX (The counter)
    mov bx, ax      ; Set the counter
    dec bx          ; Decrement AX because will add \0 at the end

.loop:

    cmp bx, 0       ; If AL is 0
    je .end         ; Then jump end

    call getc       ; Get char
    call putc       ; Print the char

    cmp al, CR      ; If is CR(ENTER)
    je .end         ; Then jump end

.loop_end:
    mov [di], al    ; Value at pointer = the char
    inc di          ; Increment the pointer
    dec bx          ; Decrement the counter
    jmp .loop       ; Loop back
.end:
    xor bx, bx
    mov [di], bx
    inc di          ; Add the zero terminator
    pop bx          ; Restore BX
    pop ax          ; Restore AX
    pop di          ; Restore DI
    ret

; Get string from keyboard
; @return AH The scancode of the key
; @return AL The ASCII code of the key
getc:
    mov ah, 0
    int 0x16
    ret


; Print string in TTY
; @param SI The address of the string
puts:
    push si         ; Save SI
    push ax         ; Save AX

.while:
    lodsb           ; Load next char in string in AL
    or al, al       ; Check if AL is \0
    jz .end         ; Jump to .end if process end

    call putc       ; Draw the char
    jmp .while

.end:
    pop ax          ; Restore AX
    pop si          ; Restore SI
    ret

; Print char in TTY
; @param AH The char to draw
putc:
    push ax          ; Save AX

    mov  ah, 0xE     ; Mode Write TTY

    cmp  al, LF      ; If AX == CR or LF
    je   .line       ; Then jump to draw new line
    cmp  al, CR
    je   .line

    int  0x10        ; Draw the char
    jmp  .end        ; Stop
.line:
    mov  al, CR      ; Draw CR
    int  0x10        
    mov  al, LF      ; Draw LF
    int  0x10

.end:
    pop  ax          ; Restore AX
    ret