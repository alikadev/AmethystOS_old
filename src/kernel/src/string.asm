bits 16
section .text

%include "string.inc"

; Compare 2 strings.
; If s1 = s2: Carry clear
; If s1 = s2: Carray Set
; @param SI s1
; @param DI s2
; @return Carry:Set if equals, Carry:Clear if not equals
streq:
	push si
	push di
.loop:
    mov al, [si]
    mov bl, [di]
    cmp al, bl
    jne .notequal

    or al, al
    jz .done

    inc si
    inc di
    jmp .loop

.notequal:
    clc
    pop di
    pop si
    ret
.done:
    stc
    pop di
    pop si
    ret